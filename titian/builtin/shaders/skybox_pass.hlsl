// Vertex shader
cbuffer VS_CB : register(b0)
{
    float4x4 vp_matrix;
}

struct VSOut
{
	float4 world : SV_Position;
	float3 textur : VS_Texture;
};

VSOut v_shader(float3 position : KL_Position)
{
    VSOut data;
    data.world = mul(float4(position, 0.0f), vp_matrix).xyww;
	data.textur = position;
	return data;
}

// Pixel shader
SamplerState skybox_sampler : register(s0);
TextureCube skybox_texture : register(t0);

float4 p_shader(VSOut data) : SV_Target
{
    return skybox_texture.Sample(skybox_sampler, data.textur);
}
