#pragma once

#include "klibrary.h"


namespace titian {
    class DepthStates
    {
    public:
        kl::dx::DepthState disabled = nullptr;
        kl::dx::DepthState enabled = nullptr;
        
        DepthStates(kl::GPU* gpu);
    };
}
