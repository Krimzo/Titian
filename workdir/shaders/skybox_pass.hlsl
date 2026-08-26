struct VS_OUT
{
    float4 position : SV_Position;
    float3 uv : VS_UV;
};

float4x4 VP;

TextureCube SKYBOX_TEXTURE : register(t0);

SamplerState SKYBOX_SAMPLER : register(s0);

VS_OUT v_shader(float3 position : KL_Position)
{
    VS_OUT data;
    data.position = mul(float4(position, 0.0f), VP).xyww;
    data.uv = position;
    return data;
}

float4 p_shader(VS_OUT data) : SV_Target0
{
    return SKYBOX_TEXTURE.Sample(SKYBOX_SAMPLER, data.uv);
}
