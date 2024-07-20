﻿#pragma once

#include "standard/standard.h"


namespace titian {
    class RasterStates
    {
    public:
        dx::RasterState wireframe = nullptr;
        dx::RasterState solid_cull = nullptr;
        dx::RasterState solid = nullptr;
        dx::RasterState shadow = nullptr;

        RasterStates(kl::GPU* gpu);
    };
}
