#include "graphics/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::depth_view kl::gpu::new_depth_view(const dx::texture texture, const dx::depth_view_descriptor* descriptor)
{
    dx::depth_view view = nullptr;

    if (const long result = device_->CreateDepthStencilView(texture, descriptor, &view); warning_check(!view, format("Failed to create depth stencil view. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(view);

    return view;
}

void kl::gpu::clear_depth_view(const dx::depth_view view, const float depth, const uint8_t stencil) const
{
    context_->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
