#include "graphics/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::shader_view kl::gpu::new_shader_view(const dx::resource resource, const dx::shader_view_descriptor* descriptor)
{
    dx::shader_view view = nullptr;

    if (const long result = device_->CreateShaderResourceView(resource, descriptor, &view); warning_check(!view, format("Failed to create shader view. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(view);

    return view;
}

void kl::gpu::bind_pixel_shader_view(const dx::shader_view view, const int slot) const
{
    context_->PSSetShaderResources(slot, 1, &view);
}

void kl::gpu::bind_compute_shader_view(const dx::shader_view view, const int slot) const
{
    context_->CSSetShaderResources(slot, 1, &view);
}
