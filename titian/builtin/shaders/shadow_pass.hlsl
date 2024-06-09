// Vertex shader
cbuffer VS_CB : register(b0)
{
	float4x4 WVP;
}

float4 v_shader(const float3 position : KL_Position) : SV_Position
{
	return mul(float4(position, 1.0f), WVP);
}

// Pixel shader
void p_shader()
{}
