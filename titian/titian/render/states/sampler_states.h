#pragma once

#include "standard/standard.h"


namespace titian {
    class SamplerStates
    {
    public:
        dx::SamplerState linear = nullptr;
        dx::SamplerState non_linear = nullptr;
        dx::SamplerState shadow = nullptr;
        
        SamplerStates(kl::GPU* gpu);
    };
}
