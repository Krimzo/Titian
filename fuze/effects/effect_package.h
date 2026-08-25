#pragma once

#include "titian.h"


namespace titian
{
struct EffectPackage
{
    float current_time = 0.0f;
    float media_start = 0.0f;
    float media_end = 0.0f;

    EffectPackage();
};
}
