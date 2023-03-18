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
cbuffer PS_CB : register(b0)
{
    float4 color;
}

float4 p_shader(float4 position : SV_Position) : SV_Target
{
    return color;
}
