#pragma once

#include "standard.h"


namespace titian {
    struct DepthStates
    {
        dx::DepthState disabled;
        dx::DepthState enabled;
        dx::DepthState only_compare;
        
        void init(kl::GPU& gpu);
    };
}
