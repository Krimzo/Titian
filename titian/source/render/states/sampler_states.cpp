#include "titian.h"


void titian::SamplerStates::init(kl::GPU& gpu)
{
    dx::SamplerStateDescriptor shadow_sampler_descriptor{};
    shadow_sampler_descriptor.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    shadow_sampler_descriptor.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    shadow_sampler_descriptor.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    shadow_sampler_descriptor.BorderColor[0] = 1.0f;
    shadow_sampler_descriptor.BorderColor[1] = 1.0f;
    shadow_sampler_descriptor.BorderColor[2] = 1.0f;
    shadow_sampler_descriptor.BorderColor[3] = 1.0f;
    shadow_sampler_descriptor.ComparisonFunc = D3D11_COMPARISON_LESS;

    linear = gpu.create_sampler_state(true, false);
    non_linear = gpu.create_sampler_state(false, false);
    shadow = gpu.create_sampler_state(&shadow_sampler_descriptor);

    kl::assert(linear, "Failed to init LINEAR sampler state");
    kl::assert(non_linear, "Failed to init NON_LINEAR sampler state");
    kl::assert(shadow, "Failed to init SHADOW sampler state");
}
