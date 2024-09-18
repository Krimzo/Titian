float4x4 WVP;
float4 SOLID_COLOR;

float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return mul(float4(position, 1.0f), WVP);
}

float4 p_shader(float4 position : SV_Position) : SV_Target0
{
    return SOLID_COLOR;
}
