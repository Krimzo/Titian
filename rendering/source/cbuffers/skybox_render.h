#pragma once

#include "klib.h"


struct skybox_render_vs_cb
{
    kl::float4x4 vp_matrix;
};

inline kl::dx::buffer skybox_render_vs_const_buffer = nullptr;
