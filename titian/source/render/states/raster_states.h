#pragma once

#include "standard.h"


namespace titian {
    struct RasterStates
    {
        dx::RasterState wireframe;
        dx::RasterState solid_cull;
        dx::RasterState solid;
        dx::RasterState shadow;

        void init(kl::GPU& gpu);
    };
}
