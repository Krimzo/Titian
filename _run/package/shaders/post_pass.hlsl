// Vertex shader
struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 textur : VS_TEXTURE;
};

VS_OUT v_shader(const float3 position : KL_Position, const float2 textur : KL_Texture)
{
    VS_OUT data;
    data.position = float4(position, 1.0f);
    data.textur = textur;
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float2 FRAME_SIZE;
    float4x4 CUSTOM_DATA;
};

Texture2D GAME_COLOR_TEXTURE : register(t0);
Texture2D GAME_DEPTH_TEXTURE : register(t1);
Texture2D EDITOR_PICKING_TEXTURE : register(t2);

SamplerState LINEAR_SAMPLER : register(s0);
SamplerState NON_LINEAR_SAMPLER : register(s1);

float4 p_shader(const VS_OUT data) : SV_Target0
{
    return GAME_COLOR_TEXTURE[data.position.xy];
}
