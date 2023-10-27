export module raster_states;

export import klib;

export namespace titian {
    class RasterStates
    {
    public:
        kl::dx::RasterState wireframe = nullptr;
        kl::dx::RasterState solid_cull = nullptr;
        kl::dx::RasterState solid = nullptr;
        kl::dx::RasterState shadow = nullptr;

        RasterStates(kl::GPU* gpu)
        {
            kl::dx::RasterStateDescriptor shadow_raster_descriptor = {};
            shadow_raster_descriptor.FillMode = D3D11_FILL_SOLID;
            shadow_raster_descriptor.CullMode = D3D11_CULL_BACK;
            shadow_raster_descriptor.FrontCounterClockwise = true;
            shadow_raster_descriptor.MultisampleEnable = true;
            shadow_raster_descriptor.AntialiasedLineEnable = true;
            shadow_raster_descriptor.SlopeScaledDepthBias = 3.0f;

            wireframe = gpu->create_raster_state(true, false);
            solid = gpu->create_raster_state(false, false);
            solid_cull = gpu->create_raster_state(false, true, true);
            shadow = gpu->create_raster_state(&shadow_raster_descriptor);

            kl::assert(wireframe, "Failed to init WIREFRAME raster state.");
            kl::assert(solid, "Failed to init SOLID raster state.");
            kl::assert(solid_cull, "Failed to init SOLID_CULL raster state.");
            kl::assert(shadow, "Failed to init SHADOW raster state.");
        }
    };
}
