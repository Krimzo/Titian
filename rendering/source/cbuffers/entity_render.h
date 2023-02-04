#pragma once

#include "klib.h"


struct entity_render_vs_cb
{
    kl::mat4  w_matrix;
    kl::mat4 vp_matrix;

    kl::mat4 vp_light_matrices[kl::directional_light::MAP_COUNT];
};

struct entity_render_ps_cb
{
    kl::float4 object_color;
    kl::float4  object_data;
    kl::float4 texture_info;

    kl::float4 camera_position;
    kl::float4      light_data;

    kl::mat4 v_matrix;
    kl::float4 cascade_distances;
};
