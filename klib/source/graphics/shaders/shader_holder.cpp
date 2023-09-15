#include "klib.h"


// Construct
template<typename S>
kl::shader_holder<S>::shader_holder(kl::gpu* gpu)
    : gpu(gpu)
{}

template<typename S>
kl::shader_holder<S>::shader_holder(kl::gpu* gpu, const S& shader)
    : gpu(gpu), shader(shader)
{}

// Get
template<typename S>
kl::shader_holder<S>::operator S() const
{
    return shader;
}

template<typename S>
kl::shader_holder<S>::operator bool() const
{
    return (bool) shader;
}

// CBuffer
template<typename S>
void kl::shader_holder<S>::update_cbuffer(const void* data, UINT byte_size)
{
    // Fix size scaling
    const UINT size_mod = byte_size % 16;
    if (size_mod != 0) {
        byte_size += (16 - size_mod);
    }

    // Regenerate buffer if needed
    const UINT buffer_size = cbuffer ? gpu->buffer_size(cbuffer) : 0;
    if (byte_size != buffer_size) {
        cbuffer = gpu->create_const_buffer(byte_size);
    }

    // Write data
    gpu->write_to_resource(cbuffer, data, byte_size);

    // Rebind cbuffer
    if constexpr (std::is_same<S, dx::vertex_shader>::value) {
        gpu->bind_cb_for_vertex_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<S, dx::geometry_shader>::value) {
        gpu->bind_cb_for_geometry_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<S, dx::pixel_shader>::value) {
        gpu->bind_cb_for_pixel_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<S, dx::compute_shader>::value) {
        gpu->bind_cb_for_compute_shader(cbuffer, 0);
    }
}

// Allowed types
template struct kl::shader_holder<kl::dx::vertex_shader>;
template struct kl::shader_holder<kl::dx::geometry_shader>;
template struct kl::shader_holder<kl::dx::pixel_shader>;
template struct kl::shader_holder<kl::dx::compute_shader>;
