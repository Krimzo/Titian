#pragma once

#include "globals.h"

namespace titian
{
struct BlendStates
{
    dx::BlendState disabled;
    dx::BlendState enabled;

    BlendStates();
};
} // namespace titian
