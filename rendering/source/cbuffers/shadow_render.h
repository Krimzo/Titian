#pragma once

#include "klib.h"


struct shadow_render_vs_cb
{
    kl::float4x4 WVP;
};

inline kl::dx::buffer shadow_render_vs_const_buffer = nullptr;
