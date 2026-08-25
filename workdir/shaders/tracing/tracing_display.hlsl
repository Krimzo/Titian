struct VS_OUT
{
	float4 position : SV_Position;
	float2 uv : VS_UV;
};

Texture2D SOURCE_TEXTURE : register(t0);

SamplerState SOURCE_SAMPLER : register(s0);

VS_OUT v_shader(float3 position : KL_Position, float2 uv : KL_UV)
{
	VS_OUT data;
	data.position = float4(position, 1.0f);
	data.uv = uv;
	return data;
}

float4 p_shader(VS_OUT data) : SV_Target0
{
	return SOURCE_TEXTURE.Sample(SOURCE_SAMPLER, data.uv);
}
