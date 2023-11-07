#pragma once

#include "klibrary.h"


namespace titian {
    class SamplerStates
    {
    public:
        kl::dx::SamplerState linear = nullptr;
        kl::dx::SamplerState shadow = nullptr;
        
        SamplerStates(kl::GPU* gpu);
    };
}
