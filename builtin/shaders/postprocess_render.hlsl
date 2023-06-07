// Vertex shader
struct vs_out
{
    float4 screen : SV_Position;
    float2 textur :  VS_Texture;
};

vs_out v_shader(float3 screen : KL_Position, float2 textur : KL_Texture)
{
    vs_out data;
    data.screen = float4(screen, 1);
    data.textur = textur;
	return data;
}

// Pixel shader
cbuffer ps_cb : register(b0)
{
    float4 selected_index;
    float4 outline_color;
};

Texture2D index_texture : register(t0);

static const int outline_thickness = 1;

float4 p_shader(vs_out vs_data) : SV_Target
{
    const float pixel_index = index_texture[vs_data.screen.xy].x;
    if (pixel_index == selected_index.x) { discard; }

    for (int y = -outline_thickness; y <= outline_thickness; y++) {
        for (int x = -outline_thickness; x <= outline_thickness; x++) {
            const float index = index_texture[vs_data.screen.xy + int2(x, y)].x;
            if (index == selected_index.r) {
                return outline_color;
            }
        }
    }

    discard;
    return 0;
}
