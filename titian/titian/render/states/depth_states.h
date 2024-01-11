#pragma once

#include "klibrary.h"


namespace titian {
    class DepthStates
    {
    public:
        kl::dx::DepthState disabled = nullptr;
        kl::dx::DepthState enabled = nullptr;
        kl::dx::DepthState only_compare = nullptr;
        
        DepthStates(kl::GPU* gpu);
    };
}
