RWTexture2D<float4> first_texture : register(u0);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    first_texture[thread_id.xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
}
