#pragma once

#include "klib.h"


struct postprocess_ps_cb
{
    kl::float4 selected_index;
    kl::float4  outline_color;
};

inline kl::dx::buffer postprocess_ps_const_buffer = nullptr;