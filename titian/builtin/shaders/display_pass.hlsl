// Vertex shader
float4 v_shader(const float3 position : KL_Position) : SV_Position
{
    return float4(position, 1);
}

// Pixel shader
Texture2D source_texture : register(t0);

float4 p_shader(const float4 position : SV_Position) : SV_Target
{
    return source_texture[position.xy];
}
