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
}

float4 p_shader(const VS_OUT data) : SV_Target
{
    static const float3 sun_direction = normalize(float3(-0.5f, -0.75f, 1.0f));
    
    const float3 normal = normalize(data.normal);
    const float diffuse = max(dot(normal, -sun_direction), 0.0f);
    return OBJECT_COLOR * diffuse;
}
