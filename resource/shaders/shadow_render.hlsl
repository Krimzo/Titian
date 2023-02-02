// Vertex shader
cbuffer VS_CB : register(b0)
{
	matrix WVP;
}

float4 v_shader(float3 position : KL_Position) : SV_Position
{
	return mul(float4(position, 1), WVP);
}

// Pixel shader
float4 p_shader(float4 screen : SV_Position) : SV_Target
{
	return 0;
}
