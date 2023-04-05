#pragma once

#include "klib.h"


struct collider_render_vs_cb
{
    kl::float4x4 wvp_matrix;
};

struct collider_render_ps_cb
{
    kl::float4 object_color; // (color.r, color.g, color.b, none)
};
