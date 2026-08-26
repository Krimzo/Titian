#pragma once

#include "globals.h"

namespace titian
{
struct RasterStates
{
    dx::RasterState wireframe;
    dx::RasterState solid_cull;
    dx::RasterState solid;
    dx::RasterState shadow;

    RasterStates();
};
} // namespace titian
