#pragma once

#include "klib.h"


struct gui_state
{
    kl::int2 viewport_size = {};
    bool is_viewport_focused = false;

    int gizmo_operation = 0;
    int gizmo_mode = 1;
};
