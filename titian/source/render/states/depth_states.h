#pragma once

#include "standard.h"


namespace titian {
    class DepthStates
    {
    public:
        dx::DepthState disabled;
        dx::DepthState enabled;
        dx::DepthState only_compare;
        
        DepthStates(kl::GPU* gpu);
    };
}
