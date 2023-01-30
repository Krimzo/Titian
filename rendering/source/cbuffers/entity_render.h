#pragma once

#include "klib.h"


struct entity_render_vs_cb
{
    kl::mat4  w_matrix;
    kl::mat4 vp_matrix;
};

struct entity_render_ps_cb
{
    kl::float4 object_color;
    kl::float4  object_data;

    kl::float4 camera_position;
    kl::float4      light_data;
};
