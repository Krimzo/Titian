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
SamplerState shadow_sampler : register(s0);
Texture2D    shadow_texture : register(t0);

static const float near = 0.01f;
static const float far  = 80.0f;

float linearize_depth(float depth)
{
    return (2 * near * far) / (far + near - (depth * 2 - 1) * (far - near));
}

float4 p_shader(vs_out vs_data) : SV_Target
{
    vs_data.textur.y = 1 - vs_data.textur.y;
    const float depth = shadow_texture.Sample(shadow_sampler, vs_data.textur).r;
	return linearize_depth(depth) / far;
}
