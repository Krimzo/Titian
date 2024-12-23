#include "fuze.h"


titian::ImageEffect::ImageEffect( StringRef const& source )
{
    for ( auto& value : custom_data.data )
        value = 0.0f;

    String full_source = R"(
cbuffer CS_CB : register(b0)
{
    float4x4 custom_data;
	float2 frame_size;
	float current_time;
	float media_start;
	float media_end;
};

RWTexture2D<float4> SOURCE : register(u0);
RWTexture2D<float4> TARGET : register(u1);

int2 convert_coords(float2 coords)
{
	return (int2) coords;
}

#custom_source

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
	if (thread_id.x < uint(frame_size.x) && thread_id.y < uint(frame_size.y))
		func(thread_id.xy);
}
)";
    kl::replace_all( full_source, "#custom_source", source );

    kl::GPU const& gpu = AppLayer::get().gpu;
    compute_shader = gpu.create_compute_shader( full_source );

    if constexpr ( kl::IS_DEBUG )
        kl::assert( compute_shader, "Failed to init image effect shader" );
}

void titian::ImageEffect::apply( EffectPackage const& package, Frame const& frame )
{
    if ( !frame.texture )
        return;

    if ( needs_copy() )
        temp_frame.resize( frame.size() );

    kl::GPU& gpu = AppLayer::get().gpu;
    gpu.bind_compute_shader( compute_shader.shader );
    gpu.bind_access_view_for_compute_shader( frame.access_view, 0 );
    gpu.bind_access_view_for_compute_shader( temp_frame.access_view, 1 );

    struct alignas(16) CB
    {
        Float4x4 custom_data;
        Float2 frame_size;
        float current_time;
        float media_start;
        float media_end;
    };

    CB cb{};
    cb.custom_data = custom_data;
    cb.frame_size = frame.size();
    cb.current_time = package.current_time;
    cb.media_start = package.media_start;
    cb.media_end = package.media_end;
    compute_shader.upload( cb );

    Int2 dispatch_size = (frame.size() / 32) + Int2( 1 );
    gpu.dispatch_compute_shader( dispatch_size.x, dispatch_size.y, 1 );

    gpu.unbind_access_view_for_compute_shader( 1 );
    gpu.unbind_access_view_for_compute_shader( 0 );
    gpu.unbind_compute_shader();

    if ( needs_copy() )
        gpu.copy_resource( frame.texture, temp_frame.texture );
}
