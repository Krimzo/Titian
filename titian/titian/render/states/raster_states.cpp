#include "main.h"


titian::RasterStates::RasterStates(kl::GPU* gpu)
{
    dx::RasterStateDescriptor wireframe_raster_descriptor{};
    wireframe_raster_descriptor.FillMode = D3D11_FILL_WIREFRAME;
    wireframe_raster_descriptor.CullMode = D3D11_CULL_NONE;
    wireframe_raster_descriptor.FrontCounterClockwise = true;
    wireframe_raster_descriptor.MultisampleEnable = true;
    wireframe_raster_descriptor.AntialiasedLineEnable = true;
    wireframe_raster_descriptor.DepthClipEnable = false;

    dx::RasterStateDescriptor shadow_raster_descriptor{};
    shadow_raster_descriptor.FillMode = D3D11_FILL_SOLID;
    shadow_raster_descriptor.CullMode = D3D11_CULL_BACK;
    shadow_raster_descriptor.FrontCounterClockwise = true;
    shadow_raster_descriptor.MultisampleEnable = true;
    shadow_raster_descriptor.AntialiasedLineEnable = true;
    shadow_raster_descriptor.DepthClipEnable = false;
    shadow_raster_descriptor.SlopeScaledDepthBias = 3.0f;

    wireframe = gpu->create_raster_state(&wireframe_raster_descriptor);
    solid = gpu->create_raster_state(false, false);
    solid_cull = gpu->create_raster_state(false, true, true);
    shadow = gpu->create_raster_state(&shadow_raster_descriptor);

    kl::assert(wireframe, "Failed to init WIREFRAME raster state.");
    kl::assert(solid, "Failed to init SOLID raster state.");
    kl::assert(solid_cull, "Failed to init SOLID_CULL raster state.");
    kl::assert(shadow, "Failed to init SHADOW raster state.");
}
