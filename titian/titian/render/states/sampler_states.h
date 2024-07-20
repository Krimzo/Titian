﻿#pragma once

#include "standard/standard.h"


namespace titian {
    class SamplerStates
    {
    public:
        kl::dx::SamplerState linear = nullptr;
        kl::dx::SamplerState non_linear = nullptr;
        kl::dx::SamplerState shadow = nullptr;
        
        SamplerStates(kl::GPU* gpu);
    };
}
