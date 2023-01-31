// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix  w_matrix;
    matrix vp_matrix;
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
    float4 object_color;
    float4  object_data; // (has_texture?, reflection_factor, refraction_factor, refraction_index)
    
    float4 camera_position;
    float4      light_data; // (sun.x, sun.y, sun.z, ambient_factor)
};

SamplerState entity_sampler : register(s0);
Texture2D    entity_texture : register(t0);

SamplerState skybox_sampler : register(s1);
TextureCube  skybox_texture : register(t1);

float4 p_shader(vs_out vs_data) : SV_Target
{
    // Setup
    vs_data.normal = normalize(vs_data.normal);

    // Reflection calculations
    const float3 camera_pixel_direction = normalize(vs_data.world - camera_position);
    const float3 reflected_pixel_direction = reflect(camera_pixel_direction, vs_data.normal);
    const float3 reflected_sky_color = skybox_texture.Sample(skybox_sampler, reflected_pixel_direction);

    // Refraction calculations
    const float3 refracted_pixel_direction = refract(camera_pixel_direction, vs_data.normal, object_data.w);
    const float3 refracted_sky_color = skybox_texture.Sample(skybox_sampler, refracted_pixel_direction);

    // Light calculations
    const float3 reflected_sun_direction = reflect(-light_data.xyz, vs_data.normal);
    const float specular_strength = dot(camera_pixel_direction, reflected_sun_direction);

    const float ambient_factor = light_data.w;
    const float diffuse_factor = max(dot(-light_data.xyz, vs_data.normal), 0);
    const float specular_factor = pow(max(specular_strength, 0), 64) * object_data.y;

    const float light_intensity = ambient_factor + diffuse_factor + specular_factor;

    // Color calculations
    const float3 unlit_color = (object_data.x) ? entity_texture.Sample(entity_sampler, vs_data.textur) : object_color;
    const float3 lit_color = unlit_color * light_intensity;

    const float3 reflected_color = lerp(lit_color, reflected_sky_color, object_data.y);
    const float3 refracted_color = lerp(reflected_color, refracted_sky_color, object_data.z);

    // Final
    return float4(refracted_color, 1);
}
