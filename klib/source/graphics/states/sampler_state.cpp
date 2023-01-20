#include "graphics/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::sampler_state kl::gpu::new_sampler_state(const dx::sampler_state_descriptor* descriptor)
{
    dx::sampler_state state = nullptr;

    if (const long result = device_->CreateSamplerState(descriptor, &state); warning_check(!state, format("Failed to create sampler state. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(state);

    return state;
}

kl::dx::sampler_state kl::gpu::new_sampler_state(const bool linear, const bool mirror)
{
    dx::sampler_state_descriptor descriptor = {};

    descriptor.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    descriptor.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    descriptor.AddressV = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    descriptor.AddressW = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;

    return new_sampler_state(&descriptor);
}

void kl::gpu::bind_sampler_state(const dx::sampler_state state, const int slot) const
{
    context_->PSSetSamplers(slot, 1, &state);
}
