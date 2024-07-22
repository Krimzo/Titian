// Vertex shader
float4 v_shader(const float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 OUTLINE_COLOR;
    int OUTLINE_SIZE;
    uint SELECTED_COUNT;
};

Texture2D INDEX_TEXTURE : register(t0);
StructuredBuffer<float> SELECTED_INDICES : register(t1);

inline bool is_selected(const float value) {
    for (uint i = 0; i < SELECTED_COUNT; i++) {
        if (SELECTED_INDICES[i] == value) {
            return true;
        }
    }
    return false;
}

float4 p_shader(const float4 position : SV_Position) : SV_Target0
{
    const float pixel_index = INDEX_TEXTURE[position.xy].r;
    if (is_selected(pixel_index)) {
        discard;
    }

    for (int2 offset = -OUTLINE_SIZE; offset.y <= OUTLINE_SIZE; offset.y++) {
        for (offset.x = -OUTLINE_SIZE; offset.x <= OUTLINE_SIZE; offset.x++) {
            if (is_selected(INDEX_TEXTURE[position.xy + offset].r)) {
                return OUTLINE_COLOR;
            }
        }
    }

    discard;
    return 0.0f;
}