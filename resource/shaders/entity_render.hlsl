// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix  w_matrix;
    matrix vp_matrix;

    matrix vp_light_matrix;
};

struct vs_out
{
    float4 screen : SV_Position;
    float3  world :    VS_World;
    float2 textur :  VS_Texture;
    float3 normal :   VS_Normal;
    
    float4 light_coords : VS_Light;
};

vs_out v_shader(float3 position : KL_Position, float2 textur : KL_Texture, float3 normal : KL_Normal)
{
    vs_out data;

    data.world = mul(float4(position, 1), w_matrix).xyz;
    data.screen = mul(float4(data.world, 1), vp_matrix);
    data.textur = textur;
    data.normal = mul(float4(normal, 0), w_matrix).xyz;

    data.light_coords = mul(float4(position, 1), mul(w_matrix, vp_light_matrix));
    data.light_coords.xy *= float2(0.5f, -0.5f);
    data.light_coords.xy += float2(0.5f, 0.5f) * data.light_coords.w;

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 object_color;
    float4  object_data; // (has_texture?, reflection_factor, refraction_factor, refraction_index)
    
    float4 camera_position;
    float4      light_data; // (sun.x, sun.y, sun.z, ambient_factor)

    float4 shadow_data; // (texel_size.x, texel_size.y, none, none)
};

SamplerState skybox_sampler : register(s0);
TextureCube  skybox_texture : register(t0);

SamplerState shadow_sampler : register(s1);
Texture2D    shadow_texture : register(t1);

SamplerState entity_sampler : register(s2);
Texture2D    entity_texture : register(t2);

float4 p_shader(vs_out vs_data) : SV_Target
{
    // Setup
    vs_data.normal = normalize(vs_data.normal);
    vs_data.light_coords /= vs_data.light_coords.w;

    // Reflection calculations
    const float3 camera_pixel_direction = normalize(vs_data.world - camera_position);
    const float3 reflected_pixel_direction = reflect(camera_pixel_direction, vs_data.normal);
    const float3 reflected_sky_color = skybox_texture.Sample(skybox_sampler, reflected_pixel_direction);

    // Refraction calculations
    const float3 refracted_pixel_direction = refract(camera_pixel_direction, vs_data.normal, object_data.w);
    const float3 refracted_sky_color = skybox_texture.Sample(skybox_sampler, refracted_pixel_direction);

    // Shadow calculations
    float shadow_factor = 0;
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            const float depth = shadow_texture.Sample(shadow_sampler, vs_data.light_coords.xy + float2(x, y) * shadow_data.xy).r;
            shadow_factor += (depth < vs_data.light_coords.z) ? 0 : 1;
        }
    }
    shadow_factor *= 0.111111f;

    // Light calculations
    const float3 reflected_sun_direction = reflect(-light_data.xyz, vs_data.normal);
    const float specular_strength = dot(camera_pixel_direction, reflected_sun_direction);

    const float ambient_factor = light_data.w;
    const float diffuse_factor = max(dot(-light_data.xyz, vs_data.normal), 0);
    const float specular_factor = pow(max(specular_strength, 0), 64) * object_data.y;

    const float light_intensity = ambient_factor + (diffuse_factor + specular_factor) * shadow_factor;

    // Color calculations
    const float3 unlit_color = (object_data.x) ? entity_texture.Sample(entity_sampler, vs_data.textur) : object_color;
    const float3 lit_color = unlit_color * light_intensity;

    const float3 reflected_color = lerp(lit_color, reflected_sky_color, object_data.y);
    const float3 refracted_color = lerp(reflected_color, refracted_sky_color, object_data.z);

    // Final
    return float4(refracted_color, 1);
}
