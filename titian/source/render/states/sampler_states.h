#pragma once

#include "standard.h"


namespace titian {
    struct SamplerStates
    {
        dx::SamplerState linear;
        dx::SamplerState non_linear;
        dx::SamplerState shadow;
        
        SamplerStates();
    };
}
