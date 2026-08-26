#pragma once

#include "globals.h"

namespace titian
{
struct SamplerStates
{
    dx::SamplerState linear;
    dx::SamplerState non_linear;
    dx::SamplerState shadow;

    SamplerStates();
};
} // namespace titian
