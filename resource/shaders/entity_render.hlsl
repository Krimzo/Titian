// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix     w_matrix;
    matrix    vp_matrix;
    float4 vs_misc_data;
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
    data.world = mul(float4(position, 1.0f), w_matrix).xyz;
    data.screen = mul(float4(data.world, 1.0f), vp_matrix);
    data.textur = textur;
    data.normal = mul(float4(normal, 0.0f), w_matrix).xyz;
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4  object_color;
    float4 sun_direction;
    float4  ps_misc_data;
};

SamplerState texture_sampler : register(s0);
Texture2D entity_texture : register(t0);

float4 p_shader(vs_out vs_data) : SV_Target
{
    float3 pixel = object_color;

    if (ps_misc_data.x) {
        pixel = entity_texture.Sample(texture_sampler, vs_data.textur);
    }

    const float light_intensity = max(dot(-sun_direction.xyz, vs_data.normal), 0.0f);
    pixel *= light_intensity;
    
    return float4(pixel, 1.0f);
}
