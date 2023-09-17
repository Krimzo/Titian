export module setup_raster_states;

export import editor_setup;

export void setup_raster_states(EditorState* state)
{
    kl::dx::RasterStateDescriptor shadow_raster_descriptor = {};
    shadow_raster_descriptor.FillMode = D3D11_FILL_SOLID;
    shadow_raster_descriptor.CullMode = D3D11_CULL_BACK;
    shadow_raster_descriptor.FrontCounterClockwise = true;
    shadow_raster_descriptor.MultisampleEnable = true;
    shadow_raster_descriptor.AntialiasedLineEnable = true;
    shadow_raster_descriptor.SlopeScaledDepthBias = 3.0f;

    state->raster_states.wireframe = state->gpu->create_raster_state(true, false);
    state->raster_states.solid = state->gpu->create_raster_state(false, false);
    state->raster_states.solid_cull = state->gpu->create_raster_state(false, true);
    state->raster_states.shadow = state->gpu->create_raster_state(&shadow_raster_descriptor);
}
