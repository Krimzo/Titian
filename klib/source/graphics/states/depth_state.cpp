#include "graphics/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::depth_state kl::gpu::new_depth_state(const dx::depth_state_descriptor* descriptor)
{
    dx::depth_state state = nullptr;

    if (const long result = device_->CreateDepthStencilState(descriptor, &state); warning_check(!state, format("Failed to create depth stencil state. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(state);

    return state;
}

kl::dx::depth_state kl::gpu::new_depth_state(const bool depth, const bool stencil, const bool mask)
{
    dx::depth_state_descriptor descriptor = {};

    if (depth) {
        descriptor.DepthEnable = true;
        descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        descriptor.DepthFunc = D3D11_COMPARISON_LESS;
    }

    if (stencil) {
        descriptor.StencilEnable = true;
        descriptor.StencilReadMask = 0xFF;
        descriptor.StencilWriteMask = 0xFF;

        if (mask) {
            descriptor.DepthEnable = false;
            descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
            descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
        }
        else {
            descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
            descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        }
    }

    return new_depth_state(&descriptor);
}

void kl::gpu::bind_depth_state(const dx::depth_state state) const
{
    context_->OMSetDepthStencilState(state, 0xFF);
}
