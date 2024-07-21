#pragma once

#include "standard.h"


namespace titian {
    using namespace standard;
}

namespace titian {
    class DepthStates
    {
    public:
        dx::DepthState disabled = nullptr;
        dx::DepthState enabled = nullptr;
        dx::DepthState only_compare = nullptr;
        
        DepthStates(kl::GPU* gpu);
    };
}
