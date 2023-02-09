#include "setup/setup_state.h"


void setup_sampler_states(state_machine* state)
{
    kl::dx::sampler_state_descriptor shadow_sampler_descriptor = {};
    shadow_sampler_descriptor.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    shadow_sampler_descriptor.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    shadow_sampler_descriptor.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    shadow_sampler_descriptor.BorderColor[0] = 1.0f;
    shadow_sampler_descriptor.BorderColor[1] = 1.0f;
    shadow_sampler_descriptor.BorderColor[2] = 1.0f;
    shadow_sampler_descriptor.BorderColor[3] = 1.0f;
    shadow_sampler_descriptor.ComparisonFunc = D3D11_COMPARISON_LESS;
    state->sampler_states["shadow"] = state->gpu->new_sampler_state(&shadow_sampler_descriptor);

    state->sampler_states["skybox"] = state->gpu->new_sampler_state(true, false);

    state->sampler_states["entity"] = state->gpu->new_sampler_state(true, false);
}
