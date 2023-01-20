#include "graphics/gpu.h"

#include "utility/console.h"


kl::dx::buffer kl::gpu::new_const_buffer(const int byte_size)
{
    if (warning_check(byte_size % 16, "Constant buffer size has to be a multiple of 16")) {
        return nullptr;
    }

    dx::buffer_descriptor descriptor = {};
    descriptor.ByteWidth = byte_size;
    descriptor.Usage = D3D11_USAGE_DYNAMIC;
    descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    return new_buffer(&descriptor);
}

void kl::gpu::set_const_buffer_data(const dx::buffer cbuffer, const void* data) const
{
    dx::buffer_descriptor descriptor = {};
    cbuffer->GetDesc(&descriptor);
    write_to_resource(cbuffer, data, descriptor.ByteWidth);
}

void kl::gpu::bind_vertex_const_buffer(const dx::buffer cbuffer, const int slot) const
{
    context_->VSSetConstantBuffers(slot, 1, &cbuffer);
}

void kl::gpu::bind_pixel_const_buffer(const dx::buffer cbuffer, const int slot) const
{
    context_->PSSetConstantBuffers(slot, 1, &cbuffer);
}

void kl::gpu::bind_compute_const_buffer(const dx::buffer cbuffer, const int slot) const
{
    context_->CSSetConstantBuffers(slot, 1, &cbuffer);
}
