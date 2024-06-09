// Vertex shader
float4 v_shader(const float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float SELECTED_INDEX;
    int OUTLINE_SIZE;
    float4 OUTLINE_COLOR;
};

Texture2D INDEX_TEXTURE : register(t0);

float4 p_shader(const float4 position : SV_Position) : SV_Target0
{
    const float pixel_index = INDEX_TEXTURE[position.xy].r;
    if (pixel_index == SELECTED_INDEX) {
        discard;
    }

    for (int2 offset = -OUTLINE_SIZE; offset.y <= OUTLINE_SIZE; offset.y++) {
        for (offset.x = -OUTLINE_SIZE; offset.x <= OUTLINE_SIZE; offset.x++) {
            if (INDEX_TEXTURE[position.xy + offset].r == SELECTED_INDEX) {
                return OUTLINE_COLOR;
            }
        }
    }

    discard;
    return 0.0f;
}
