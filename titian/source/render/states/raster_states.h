#pragma once

#include "standard.h"


namespace titian {
    class RasterStates
    {
    public:
        dx::RasterState wireframe;
        dx::RasterState solid_cull;
        dx::RasterState solid;
        dx::RasterState shadow;

        RasterStates(kl::GPU* gpu);
    };
}
