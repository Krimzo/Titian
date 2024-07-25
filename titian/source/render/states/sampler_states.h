#pragma once

#include "standard.h"


namespace titian {
    class SamplerStates
    {
    public:
        dx::SamplerState linear;
        dx::SamplerState non_linear;
        dx::SamplerState shadow;
        
        SamplerStates(kl::GPU* gpu);
    };
}
