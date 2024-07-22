#pragma once

#include "standard.h"


namespace titian {
    using namespace standard;
}

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
