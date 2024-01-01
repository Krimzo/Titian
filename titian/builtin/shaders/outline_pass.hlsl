// Vertex shader
float4 v_shader(const float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float SELECTED_INDEX;
    float3 OUTLINE_COLOR;
};

Texture2D INDEX_TEXTURE : register(t0);

float4 p_shader(const float4 position : SV_Position) : SV_Target
{
    static const int outline_thickness = 1;
    
    const float pixel_index = INDEX_TEXTURE[position.xy].x;
    if (pixel_index == SELECTED_INDEX) {
        discard;
    }

    for (int y = -outline_thickness; y <= outline_thickness; y++) {
        for (int x = -outline_thickness; x <= outline_thickness; x++) {
            const float index = INDEX_TEXTURE[position.xy + int2(x, y)].x;
            if (index == SELECTED_INDEX) {
                return float4(OUTLINE_COLOR, 1.0f);
            }
        }
    }

    discard;
    return 0.0f;
}
