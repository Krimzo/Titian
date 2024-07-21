// Vertex shader
cbuffer VS_CB : register(b0)
{
    float4x4 VP;
}

struct VS_OUT
{
	float4 position : SV_Position;
    float3 textur : VS_Texture;
};

VS_OUT v_shader(const float3 position : KL_Position)
{
    VS_OUT data;
    data.position = mul(float4(position, 0.0f), VP).xyww;
    data.textur = position;
	return data;
}

// Pixel shader
SamplerState SKYBOX_SAMPLER : register(s0);
TextureCube SKYBOX_TEXTURE : register(t0);

float4 p_shader(const VS_OUT data) : SV_Target
{
    return SKYBOX_TEXTURE.Sample(SKYBOX_SAMPLER, data.textur);
}
