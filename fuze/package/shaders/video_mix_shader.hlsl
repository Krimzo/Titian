// Compute shader
RWTexture2D<float4> first_texture : register(u0);
RWTexture2D<float4> second_texture : register(u1);

[numthreads(32, 32, 1)]
void c_shader(const uint3 thread_id : SV_DispatchThreadID)
{
    const float4 first_color = first_texture[thread_id.xy];
    const float4 second_color = second_texture[thread_id.xy];
    const float3 out_color = lerp(first_color.xyz, second_color.xyz, second_color.w);
    first_texture[thread_id.xy] = float4(out_color, 1.0f);
}
