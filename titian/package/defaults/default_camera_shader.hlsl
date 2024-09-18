struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 textur : VS_TEXTURE;
};

float4x4 CUSTOM_DATA;
float2 FRAME_SIZE;

Texture2D<float4> COLOR_TEXTURE : register(t0);
Texture2D<float4> DEPTH_TEXTURE : register(t1);
Texture2D<float> INDEX_TEXTURE : register(t2);

SamplerState LINEAR_SAMPLER : register(s0);
SamplerState NON_LINEAR_SAMPLER : register(s1);

VS_OUT v_shader(float3 position : KL_Position, float2 textur : KL_Texture)
{
    VS_OUT data;
    data.position = float4(position, 1.0f);
    data.textur = textur;
    return data;
}

float4 p_shader(VS_OUT data) : SV_Target0
{
    return COLOR_TEXTURE[data.position.xy];
}
