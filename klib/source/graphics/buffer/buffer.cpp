#include "graphics/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::buffer kl::gpu::new_buffer(const dx::buffer_descriptor* descriptor, const dx::subresource_descriptor* subresource_data)
{
    dx::buffer buffer = nullptr;

    if (const long result = device_->CreateBuffer(descriptor, subresource_data, &buffer); warning_check(!buffer, format("Failed to create gpu buffer. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(buffer);

    return buffer;
}

kl::dx::buffer kl::gpu::new_structured_buffer(const void* data, const int element_count, const int element_size, const bool has_unordered_access, const bool cpu_read)
{
    dx::buffer_descriptor descriptor;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL);
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descriptor.CPUAccessFlags = cpu_read ? D3D11_CPU_ACCESS_READ : NULL;
    descriptor.StructureByteStride = element_size;
    descriptor.ByteWidth = element_count * element_size;

    dx::subresource_descriptor subresource = {};
    subresource.pSysMem = data;

    return new_buffer(&descriptor, &subresource);
}

kl::dx::buffer kl::gpu::new_staging_buffer(const dx::buffer buffer, const int byte_size)
{
    dx::buffer_descriptor descriptor = {};
    buffer->GetDesc(&descriptor);

    dx::buffer_descriptor staging_descriptor = {};
    staging_descriptor.Usage = D3D11_USAGE_STAGING;
    staging_descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    staging_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    staging_descriptor.StructureByteStride = descriptor.StructureByteStride;
    staging_descriptor.ByteWidth = byte_size ? byte_size : descriptor.ByteWidth;

    return new_buffer(&staging_descriptor);
}
