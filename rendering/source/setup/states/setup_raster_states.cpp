#include "setup/setup_state.h"


void setup_raster_states(state_machine* state)
{
    // Custom
    const auto wireframe_raster = state->gpu->create_raster_state(true, false);
    state->raster_states["wireframe"] = wireframe_raster;

    // Render
    kl::dx::raster_state_descriptor shadow_raster_descriptor = {};
    shadow_raster_descriptor.FillMode = D3D11_FILL_SOLID;
    shadow_raster_descriptor.CullMode = D3D11_CULL_BACK;
    shadow_raster_descriptor.FrontCounterClockwise = true;
    shadow_raster_descriptor.MultisampleEnable = true;
    shadow_raster_descriptor.AntialiasedLineEnable = true;
    shadow_raster_descriptor.SlopeScaledDepthBias = 3.0f;

    const auto shadow_raster = state->gpu->create_raster_state(&shadow_raster_descriptor);
    state->raster_states["shadow"] = shadow_raster;

    const auto skybox_raster = state->gpu->create_raster_state(false, false);
    state->raster_states["skybox"] = skybox_raster;

    const auto entity_raster = state->gpu->create_raster_state(false, true);
    state->raster_states["entity"] = entity_raster;

    const auto postprocess_raster = state->gpu->create_raster_state(false, false);
    state->raster_states["postprocess"] = postprocess_raster;
}
