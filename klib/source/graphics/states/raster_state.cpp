#include "graphics/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::raster_state kl::gpu::new_raster_state(const dx::raster_state_descriptor* descriptor)
{
    dx::raster_state raster_state = nullptr;

    if (const long result = device_->CreateRasterizerState(descriptor, &raster_state); warning_check(!raster_state, format("Failed to create rasterizer state. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(raster_state);

    return raster_state;
}

kl::dx::raster_state kl::gpu::new_raster_state(const bool wireframe, const bool cull, const bool cull_back)
{
    dx::raster_state_descriptor raster_state_desc = {};

    raster_state_desc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    raster_state_desc.CullMode = cull ? (cull_back ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
    raster_state_desc.FrontCounterClockwise = true;
    raster_state_desc.MultisampleEnable = true;
    raster_state_desc.AntialiasedLineEnable = true;

    return new_raster_state(&raster_state_desc);
}

void kl::gpu::bind_raster_state(const dx::raster_state state) const
{
    context_->RSSetState(state);
}
