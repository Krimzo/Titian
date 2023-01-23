#pragma once

#include "klib.h"


struct entity_render_vs_cb
{
    kl::mat4       w_matrix;
    kl::mat4      vp_matrix;
    kl::float4 vs_misc_data;
};

struct entity_render_ps_cb
{
    kl::float4  object_color;
    kl::float4 sun_direction;
    kl::float4  ps_misc_data;
};
