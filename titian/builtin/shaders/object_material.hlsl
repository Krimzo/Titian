// Vertex shader
cbuffer VS_CB : register(b0)
{
    float4x4  w_matrix;
    float4x4 vp_matrix;
};

struct vs_out
{
    float4 screen : SV_Position;
    float3  world :    VS_World;
    float2 textur :  VS_Texture;
    float3 normal :   VS_Normal;
};

vs_out v_shader(float3 position : KL_Position, float2 textur : KL_Texture, float3 normal : KL_Normal)
{
    vs_out data;

    data.world = mul(float4(position, 1), w_matrix).xyz;
    data.screen = mul(float4(data.world, 1), vp_matrix);
    data.textur = textur;
    data.normal = mul(float4(normal, 0), w_matrix).xyz;

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 object_color; // (color.r, color.g, color.b, none)

    float4     object_material; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
    float4 object_texture_info; // (has_normal_map, has_roughness_map, none, none)
    
    float4 camera_info; // (camera.x, camera.y, camera.z, skybox?)
    float4 camera_background; // (color.r, color.g, color.b, color.a)

    float4     ambient_light; // (color.r, color.g, color.b, intensity)
    float4 directional_light; // (sun.x, sun.y, sun.z, sun_point_size)
};

SamplerState skybox_sampler : register(s0);
TextureCube  skybox_texture : register(t0);

SamplerState entity_sampler : register(s1);
Texture2D    entity_texture : register(t1);
Texture2D    normal_texture : register(t2);
Texture2D roughness_texture : register(t3);

float get_pixel_reflectivity(float reflectivity, float2 texture_coords);
float3 get_pixel_normal(float3 world_position, float3 interpolated_normal, float2 texture_coords);
vs_out compute_light_transforms(vs_out vs_data);

float4 p_shader(vs_out vs_data) : SV_Target
{
    // Setup
    vs_data.normal = normalize(vs_data.normal);
    const float3 pixel_normal = get_pixel_normal(vs_data.world, vs_data.normal, vs_data.textur);
    const float pixel_reflectivity = get_pixel_reflectivity(object_material.y, vs_data.textur);

    // Reflection calculations
    const float3 camera_pixel_direction = normalize(vs_data.world - camera_info.xyz);
    const float3 reflected_pixel_direction = reflect(camera_pixel_direction, pixel_normal);
    const float3 reflected_sky_color = camera_info.w ? skybox_texture.Sample(skybox_sampler, reflected_pixel_direction).xyz : camera_background.xyz;

    // Refraction calculations
    const float3 refracted_pixel_direction = refract(camera_pixel_direction, pixel_normal, object_material.w);
    const float3 refracted_sky_color = camera_info.w ? skybox_texture.Sample(skybox_sampler, refracted_pixel_direction).xyz : camera_background.xyz;

    // Light calculations
    const float3 reflected_sun_direction = reflect(-directional_light.xyz, pixel_normal);
    const float specular_strength = dot(camera_pixel_direction, reflected_sun_direction);

    const float3 ambient_factor = ambient_light.xyz * ambient_light.w;
    const float3 diffuse_factor = max(dot(-directional_light.xyz, pixel_normal), 0.0f);
    const float specular_factor = pow(max(specular_strength, 0), 64) * pixel_reflectivity;

    const float3 light_intensity = ambient_factor + diffuse_factor + specular_factor;

    // Color calculations
    const float3 texture_color = entity_texture.Sample(entity_sampler, vs_data.textur).xyz;
    const float3 unlit_color = lerp(object_color.xyz, texture_color, object_material.x);
    const float3 lit_color = unlit_color * light_intensity;

    const float3 reflected_color = lerp(lit_color, reflected_sky_color, pixel_reflectivity);
    const float3 refracted_color = lerp(reflected_color, refracted_sky_color, object_material.z);

    // Final
    return float4(refracted_color, 1);
}

float3 get_pixel_normal(float3 world_position, float3 interpolated_normal, float2 texture_coords)
{
    if (!object_texture_info.x) {
        return interpolated_normal;
    }

    const float3 Q1 = ddx(world_position);
    const float3 Q2 = ddy(world_position);
    const float2 st1 = ddx(texture_coords);
    const float2 st2 = ddy(texture_coords);
    const float3 T = normalize(Q1 * st2.x - Q2 * st1.x);
    const float3 B = normalize(-Q1 * st2.y + Q2 * st1.y);
    const float3x3 TBN = float3x3(T, B, interpolated_normal);
    const float3 result_normal = normalize(normal_texture.Sample(entity_sampler, texture_coords).xyz * 2 - 1);
    return normalize(mul(result_normal, TBN));
}

float get_pixel_reflectivity(float reflectivity, float2 texture_coords)
{
    if (!object_texture_info.y) {
        return reflectivity;
    }
    const float roughness = roughness_texture.Sample(entity_sampler, texture_coords).r;
    return (1 - roughness);
}
