#include "klibrary.h"


template<typename S>
kl::ShaderHolder<S>::ShaderHolder(const GPU* gpu)
    : gpu(gpu)
{}

template<typename S>
kl::ShaderHolder<S>::ShaderHolder(const GPU* gpu, const S& shader)
    : gpu(gpu), shader(shader)
{}

template<typename S>
kl::ShaderHolder<S>::operator S() const
{
    return shader;
}

template<typename S>
kl::ShaderHolder<S>::operator bool() const
{
    return bool(shader);
}

template<typename S>
void kl::ShaderHolder<S>::update_cbuffer(const void* data, UINT byte_size)
{
    assert(byte_size % 16 == 0, "CBuffers must have 16 byte alignment");

    const UINT buffer_size = cbuffer ? gpu->buffer_size(cbuffer) : 0;
    if (byte_size != buffer_size) {
        cbuffer = gpu->create_const_buffer(byte_size);
    }

    gpu->write_to_buffer(cbuffer, data, byte_size);

    if constexpr (std::is_same<S, dx::VertexShader>::value) {
        gpu->bind_cb_for_vertex_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<S, dx::GeometryShader>::value) {
        gpu->bind_cb_for_geometry_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<S, dx::PixelShader>::value) {
        gpu->bind_cb_for_pixel_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<S, dx::ComputeShader>::value) {
        gpu->bind_cb_for_compute_shader(cbuffer, 0);
    }
}

template struct kl::ShaderHolder<kl::dx::VertexShader>;
template struct kl::ShaderHolder<kl::dx::GeometryShader>;
template struct kl::ShaderHolder<kl::dx::PixelShader>;
template struct kl::ShaderHolder<kl::dx::ComputeShader>;
