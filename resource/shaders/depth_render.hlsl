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
SamplerState depth_sampler : register(s0);
Texture2D    depth_texture : register(t0);

float4 p_shader(vs_out vs_data) : SV_Target
{
    vs_data.textur.y = 1 - vs_data.textur.y;
    const float depth = depth_texture.Sample(depth_sampler, vs_data.textur).r;
	return depth;
}
