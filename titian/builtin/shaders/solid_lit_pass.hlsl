// Vertex shader
cbuffer VS_CB : register(b0)
{
    float4x4 W;
	float4x4 WVP;
}

struct VS_OUT
{
    float4 position : SV_Position;
    float3 normal : VS_Normal;
};

VS_OUT v_shader(const float3 position : KL_Position, const float3 normal : KL_Normal)
{
    VS_OUT data;
    data.position = mul(float4(position, 1.0f), WVP);
    data.normal = mul(float4(normal, 0.0f), W).xyz;
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 OBJECT_COLOR;
    float3 SUN_DIRECTION;
}

float4 p_shader(const VS_OUT data) : SV_Target0
{
    const float3 normal = normalize(data.normal);
    const float diffuse = max(dot(normal, -SUN_DIRECTION), 0.0f);
    return OBJECT_COLOR * diffuse;
}
