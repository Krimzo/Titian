#include "graphics/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::target_view kl::gpu::new_target_view(const dx::texture texture, const dx::target_view_descriptor* descriptor)
{
    dx::target_view view = nullptr;

    if (const long result = device_->CreateRenderTargetView(texture, descriptor, &view); warning_check(!view, format("Failed to create render target view. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(view);

    return view;
}

void kl::gpu::clear_target_view(const dx::target_view view, const float4& values) const
{
    context_->ClearRenderTargetView(view, values.data);
}
