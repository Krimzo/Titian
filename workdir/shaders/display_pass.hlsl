Texture2D SOURCE_TEXTURE : register(t0);

float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

float4 p_shader(float4 position : SV_Position) : SV_Target0
{
    return SOURCE_TEXTURE[position.xy];
}
