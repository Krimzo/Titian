// Vertex shader
cbuffer VS_CB : register(b0)
{
	matrix VP;
}

struct vs_out
{
	float4 world : SV_Position;
	float3 textur : VS_Texture;
};

vs_out v_shader(float3 position : KL_Position, float2 textur : KL_Texture)
{
	vs_out data;
	data.world = mul(float4(position, 0.0f), VP).xyww;
	data.textur = position;
	return data;
}

// Pixel shader
SamplerState box_sampler : register(s0);
TextureCube skybox : register(t0);

float4 p_shader(vs_out data) : SV_Target
{
	return skybox.Sample(box_sampler, data.textur);
}
