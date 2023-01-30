#pragma once


#include "klib.h"


struct ocean_render_vs_cb
{
    kl::mat4     w_matrix;
    kl::mat4    vp_matrix;
    kl::float4  time_data; // (elapsed_time, delta_time, none, none)
    kl::float4 ocean_data; // (amplitude, direction_frequency, direction_frequency, wave_shift)
};

struct ocean_render_ps_cb
{
    kl::float4  object_color;
    kl::float4 sun_direction;
};
