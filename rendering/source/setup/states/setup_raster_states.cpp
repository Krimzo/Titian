#include "setup/setup_state.h"


void setup_raster_states(state_machine* state)
{
	const auto wireframe_raster = state->gpu->new_raster_state(true, false);
	state->raster_states["wireframe"] = wireframe_raster;

	const auto skybox_raster = state->gpu->new_raster_state(false, false);
	state->raster_states["skybox"] = skybox_raster;

	const auto ocean_raster = state->gpu->new_raster_state(false, true);
	state->raster_states["ocean"] = ocean_raster;
	
	kl::dx::raster_state_descriptor shadow_raster_descriptor = {};
	shadow_raster_descriptor.FillMode = D3D11_FILL_SOLID;
	shadow_raster_descriptor.CullMode = D3D11_CULL_BACK;
	shadow_raster_descriptor.FrontCounterClockwise = true;
	shadow_raster_descriptor.MultisampleEnable = true;
	shadow_raster_descriptor.AntialiasedLineEnable = true;
	shadow_raster_descriptor.SlopeScaledDepthBias = 3.0f;

	const auto shadow_raster = state->gpu->new_raster_state(&shadow_raster_descriptor);
	state->raster_states["shadow"] = shadow_raster;

	const auto entity_raster = state->gpu->new_raster_state(false, true);
	state->raster_states["entity"] = entity_raster;
}
