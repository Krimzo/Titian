float4 OUTLINE_COLOR;
int OUTLINE_SIZE;
int SELECTED_COUNT;

Texture2D<float> INDEX_TEXTURE : register(t0);
StructuredBuffer<float> INDEX_BUFFER : register(t1);

bool is_selected(float value) {
    for (int i = 0; i < SELECTED_COUNT; i++) {
        if (INDEX_BUFFER[i] == value)
            return true;
    }
    return false;
}

float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

float4 p_shader(float4 position : SV_Position) : SV_Target0
{
    if (is_selected(INDEX_TEXTURE[position.xy]))
        discard;

    for (int2 offset = -OUTLINE_SIZE; offset.y <= OUTLINE_SIZE; offset.y++)
        for (offset.x = -OUTLINE_SIZE; offset.x <= OUTLINE_SIZE; offset.x++)
            if (is_selected(INDEX_TEXTURE[position.xy + offset]))
                return OUTLINE_COLOR;

    discard;
    return 0;
}
