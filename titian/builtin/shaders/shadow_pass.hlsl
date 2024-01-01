// Vertex shader
cbuffer VS_CB : register(b0)
{
	matrix WVP;
}

float4 v_shader(const float3 position : KL_Position) : SV_Position
{
	return mul(float4(position, 1.0f), WVP);
}

// Pixel shader
float4 p_shader(const float4 position : SV_Position) : SV_Target
{
	return 0.0f;
}
