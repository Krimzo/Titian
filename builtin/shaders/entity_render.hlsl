// Constants
#define SHADOW_CASCADE_COUNT 4

// Vertex shader
cbuffer VS_CB : register(b0)
{
    float4x4  w_matrix;
    float4x4 vp_matrix;

    float4x4 vp_light_matrices[SHADOW_CASCADE_COUNT];
};

struct vs_out
{
    float4 screen : SV_Position;
    float3  world :    VS_World;
    float2 textur :  VS_Texture;
    float3 normal :   VS_Normal;
    
    float4 light_coords[SHADOW_CASCADE_COUNT] : VS_Light;
};

vs_out v_shader(float3 position : KL_Position, float2 textur : KL_Texture, float3 normal : KL_Normal)
{
    vs_out data;

    data.world = mul(float4(position, 1), w_matrix).xyz;
    data.screen = mul(float4(data.world, 1), vp_matrix);
    data.textur = textur;
    data.normal = mul(float4(normal, 0), w_matrix).xyz;

    for (int i = 0; i < SHADOW_CASCADE_COUNT; i++) {
        data.light_coords[i] = mul(float4(position, 1), mul(w_matrix, vp_light_matrices[i]));
        data.light_coords[i].xy *= float2(0.5f, -0.5f);
        data.light_coords[i].xy += float2(0.5f, 0.5f) * data.light_coords[i].w;
    }

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 object_color; // (color.r, color.g, color.b, none)
    float4 object_index; // (index, index, index, index)

    float4     object_material; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
    float4 object_texture_info; // (has_normal_map, has_roughness_map, none, none)
    
    float4 camera_info; // (camera.x, camera.y, camera.z, none)
    matrix    v_matrix; // View matrix

    float4     ambient_light; // (color.r, color.g, color.b, intensity)
    float4 directional_light; // (sun.x, sun.y, sun.z, sun_point_size)

    float4   shadow_map_info; // (width, height, texel_width, texel_size)
    float4 cascade_distances; // (cascade_0_far, cascade_1_far, cascade_2_far, cascade_3_far)
};

struct ps_out
{
    float4 color : SV_Target0;
    float  index : SV_Target1;
};

SamplerState skybox_sampler : register(s0);
TextureCube  skybox_texture : register(t0);

SamplerState shadow_sampler : register(s1);
Texture2D  shadow_texture_0 : register(t1);
Texture2D  shadow_texture_1 : register(t2);
Texture2D  shadow_texture_2 : register(t3);
Texture2D  shadow_texture_3 : register(t4);

SamplerState entity_sampler : register(s2);
Texture2D    entity_texture : register(t5);
Texture2D    normal_texture : register(t6);
Texture2D roughness_texture : register(t7);

float3 get_pixel_normal(float3 world_position, float3 interpolated_normal, float2 texture_coords);
float get_pixel_reflectivity(float reflectivity, float2 texture_coords);

vs_out compute_light_transforms(vs_out vs_data);
float get_shadow_factor(vs_out vs_data, const float camera_z, const int half_kernel_size);
float get_pcf_shadow(Texture2D shadow_map, float3 light_coords, const int half_kernel_size);

ps_out p_shader(vs_out vs_data)
{
    // Setup
    const float3 pixel_normal = get_pixel_normal(vs_data.world, normalize(vs_data.normal), vs_data.textur);
    const float pixel_reflectivity = get_pixel_reflectivity(object_material.y, vs_data.textur);
    vs_data = compute_light_transforms(vs_data);

    // Reflection calculations
    const float3 camera_pixel_direction = normalize(vs_data.world - camera_info.xyz);
    const float3 reflected_pixel_direction = reflect(camera_pixel_direction, pixel_normal);
    const float3 reflected_sky_color = skybox_texture.Sample(skybox_sampler, reflected_pixel_direction).xyz;

    // Refraction calculations
    const float3 refracted_pixel_direction = refract(camera_pixel_direction, pixel_normal, object_material.w);
    const float3 refracted_sky_color = skybox_texture.Sample(skybox_sampler, refracted_pixel_direction).xyz;

    // Shadow calculations
    const float camera_z = abs(mul(float4(vs_data.world, 1), v_matrix).z);
    const float shadow_factor = get_shadow_factor(vs_data, camera_z, 1);

    // Light calculations
    const float3 reflected_sun_direction = reflect(-directional_light.xyz, pixel_normal);
    const float specular_strength = dot(camera_pixel_direction, reflected_sun_direction);

    const float3 ambient_factor = ambient_light.xyz * ambient_light.w;
    const float diffuse_factor = max(dot(-directional_light.xyz, pixel_normal), 0);
    const float specular_factor = pow(max(specular_strength, 0), 64) * pixel_reflectivity;

    const float3 light_intensity = ambient_factor + (diffuse_factor + specular_factor) * shadow_factor;

    // Color calculations
    const float3 texture_color = entity_texture.Sample(entity_sampler, vs_data.textur).xyz;
    const float3 unlit_color = lerp(object_color.xyz, texture_color, object_material.x);
    const float3 lit_color = unlit_color * light_intensity;

    const float3 reflected_color = lerp(lit_color, reflected_sky_color, pixel_reflectivity);
    const float3 refracted_color = lerp(reflected_color, refracted_sky_color, object_material.z);

    // Final
    ps_out out_data;
    out_data.color = float4(refracted_color, 1);
    out_data.index = object_index.r;
    return out_data;
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

vs_out compute_light_transforms(vs_out vs_data)
{
    [unroll]
    for (int i = 0; i < SHADOW_CASCADE_COUNT; i++) {
        vs_data.light_coords[i] /= vs_data.light_coords[i].w;
        vs_data.light_coords[i].z = min(vs_data.light_coords[i].z, 1);
    }
    return vs_data;
}

float get_shadow_factor(vs_out vs_data, const float camera_z, const int half_kernel_size)
{
    float pcf_value = 0;
    if (camera_z < cascade_distances.x) {
        pcf_value = get_pcf_shadow(shadow_texture_0, vs_data.light_coords[0].xyz, half_kernel_size);
    }
    else if (camera_z < cascade_distances.y) {
        pcf_value = get_pcf_shadow(shadow_texture_1, vs_data.light_coords[1].xyz, half_kernel_size);
    }
    else if (camera_z < cascade_distances.z) {
        pcf_value = get_pcf_shadow(shadow_texture_2, vs_data.light_coords[2].xyz, half_kernel_size);
    }
    else {
        pcf_value = get_pcf_shadow(shadow_texture_3, vs_data.light_coords[3].xyz, half_kernel_size);
    }
    return pcf_value;
}

float get_pcf_shadow(Texture2D shadow_map, float3 light_coords, const int half_kernel_size)
{
    static const float2 adder = 0.25f;

    float shadow_factor = 0;
    float sample_counter = 0;

    [unroll]
    for (int y = -half_kernel_size; y <= half_kernel_size; y++) {
        [unroll]
        for (int x = -half_kernel_size; x <= half_kernel_size; x++) {
            const float2 kernel_coords = float2(x, y) + adder;
            const float2 altered_coords = light_coords.xy + kernel_coords * shadow_map_info.zw;
            const float depth = shadow_map.Sample(shadow_sampler, altered_coords).r;
            
            shadow_factor += (depth < light_coords.z) ? 0 : 1;
            sample_counter += 1;
        }
    }

    return (shadow_factor / sample_counter);
}
