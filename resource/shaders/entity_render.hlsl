// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix     w_matrix;
    matrix    vp_matrix;
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
    float4  object_data;
    
    float4 camera_position;
    float4      light_data;
};

SamplerState entity_sampler : register(s0);
Texture2D entity_texture : register(t0);

SamplerState skybox_sampler : register(s1);
TextureCube skybox_texture : register(t1);

float3 get_object_base_color(vs_out vs_data);

float4 p_shader(vs_out vs_data) : SV_Target
{
    // Light calculations
    vs_data.normal = normalize(vs_data.normal);
    const float ambient_factor = light_data.w;
    const float diffuse_factor = max(dot(-light_data.xyz, vs_data.normal), 0);
    const float light_intensity = diffuse_factor + ambient_factor;

    // Reflection calculations
    const float3 camera_pixel_direction = normalize(vs_data.world - camera_position);
    const float3 reflected_pixel_direction = reflect(camera_pixel_direction, vs_data.normal);
    const float3 reflected_color = skybox_texture.Sample(skybox_sampler, reflected_pixel_direction);

    // Color calculations
    const float3 base_color = get_object_base_color(vs_data);
    const float3 unlit_color = lerp(reflected_color, base_color, object_data.y);
    const float3 lit_color = unlit_color * light_intensity;

    // Final
    return float4(lit_color, 1);
}

float3 get_object_base_color(vs_out vs_data)
{
    if (object_data.x) {
        return entity_texture.Sample(entity_sampler, vs_data.textur);
    }
    return object_color;
}
