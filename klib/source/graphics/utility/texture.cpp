#include "graphics/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::texture kl::gpu::get_back_buffer()
{
    dx::texture buffer = nullptr;

    if (const long result = chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &buffer); warning_check(!buffer, format("Failed to get backbuffer texture. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(buffer);
    return buffer;
}

kl::dx::texture kl::gpu::new_texture(const dx::texture_descriptor* descriptor, const dx::subresource_descriptor* subresource_data)
{
    dx::texture texture = nullptr;

    if (const long result = device_->CreateTexture2D(descriptor, subresource_data, &texture); warning_check(!texture, format("Failed to create texture. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(texture);
    return texture;
}

kl::dx::texture kl::gpu::new_texture(const image& image, const bool has_unordered_access, const bool is_target)
{
    const kl::image flipped_image = image.flip(true);

    dx::texture_descriptor descriptor = {};
    descriptor.Width = image.width();
    descriptor.Height = image.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 1;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE |
        (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL) |
        (is_target ? D3D11_BIND_RENDER_TARGET : NULL);

    dx::subresource_descriptor data = {};
    data.pSysMem = flipped_image.data();
    data.SysMemPitch = image.width() * sizeof(color);

    return new_texture(&descriptor, &data);
}

kl::dx::texture kl::gpu::new_texture(const image& front, const image& back, const image& left, const image& right, const image& top, const image& bottom)
{
    if (warning_check(!(front.size() == back.size() &&
        front.size() == left.size() &&
        front.size() == right.size() &&
        front.size() == top.size() &&
        front.size() == bottom.size()), "Sizes of the 6 given images do not match")) {
        return nullptr;
    }

    dx::texture_descriptor descriptor = {};
    descriptor.Width = front.width();
    descriptor.Height = front.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 6;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    const UINT mem_pitch = UINT(front.width() * sizeof(color));
    const dx::subresource_descriptor data[6] = {
        { right.data(),  mem_pitch, 0 },
        { left.data(),   mem_pitch, 0 },
        { top.data(),    mem_pitch, 0 },
        { bottom.data(), mem_pitch, 0 },
        { front.data(),  mem_pitch, 0 },
        { back.data(),   mem_pitch, 0 }
    };

    return new_texture(&descriptor, data);
}

kl::dx::texture kl::gpu::new_staging_texture(const dx::texture texture, const int2& size)
{
    dx::texture_descriptor descriptor = {};
    texture->GetDesc(&descriptor);

    dx::texture_descriptor staging_descriptor = {};
    staging_descriptor.Width = (size.x > 0) ? size.x : descriptor.Width;
    staging_descriptor.Height = (size.y > 0) ? size.y : descriptor.Height;
    staging_descriptor.MipLevels = 1;
    staging_descriptor.ArraySize = 1;
    staging_descriptor.Format = descriptor.Format;
    staging_descriptor.SampleDesc.Count = 1;
    staging_descriptor.Usage = D3D11_USAGE_STAGING;
    staging_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    return new_texture(&staging_descriptor);
}
