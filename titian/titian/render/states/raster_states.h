#pragma once

#include "standard/standard.h"


namespace titian {
    class RasterStates
    {
    public:
        kl::dx::RasterState wireframe = nullptr;
        kl::dx::RasterState solid_cull = nullptr;
        kl::dx::RasterState solid = nullptr;
        kl::dx::RasterState shadow = nullptr;

        RasterStates(kl::GPU* gpu);
    };
}
