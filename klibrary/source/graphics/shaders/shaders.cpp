#include "klibrary.h"


kl::CBuffer::CBuffer(const GPU* gpu)
    : gpu(gpu)
{}

void kl::CBuffer::upload(const void* data, const UINT byte_size)
{
    assert(byte_size % 16 == 0, "CBuffers must have 16 byte alignment");
    const UINT buffer_size = cbuffer ? gpu->buffer_size(cbuffer) : 0;
    if (byte_size != buffer_size) {
        cbuffer = gpu->create_const_buffer(byte_size);
    }
    gpu->write_to_buffer(cbuffer, data, byte_size, true);
}

void kl::CBuffer::bind(const ShaderType type, const int index) const
{
    switch (type)
    {
    case ShaderType::VERTEX: gpu->bind_cb_for_vertex_shader(cbuffer, index); break;
    case ShaderType::PIXEL: gpu->bind_cb_for_pixel_shader(cbuffer, index); break;
    case ShaderType::GEOMETRY: gpu->bind_cb_for_geometry_shader(cbuffer, index); break;
    case ShaderType::COMPUTE: gpu->bind_cb_for_compute_shader(cbuffer, index); break;
    }
}

kl::Shaders::Shaders(const GPU* gpu)
    : CBuffer(gpu)
{}

kl::Shaders::operator bool() const
{
    return gpu && vertex_shader && pixel_shader;
}
