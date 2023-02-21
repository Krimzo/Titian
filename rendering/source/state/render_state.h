#pragma once

#include "klib.h"


class render_state
{
    kl::ref<kl::gpu> gpu_ = {};

public:
    const kl::int2 target_size;

    // Misc data
    kl::dx::buffer screen_mesh = nullptr;

    // Scene render
    kl::dx::target_view render_target_view = nullptr;
    kl::dx::shader_view render_shader_view = nullptr;

    // Scene picking
    kl::dx::texture picking_texture = nullptr;
    kl::dx::target_view picking_target_view = nullptr;
    kl::dx::shader_view picking_shader_view = nullptr;
    kl::dx::texture picking_staging_texture = nullptr;

    render_state(kl::ref<kl::gpu> gpu, const kl::int2& size);
    ~render_state();

    render_state(render_state&) = delete;
    render_state(render_state&&) = delete;

    void operator=(render_state&) = delete;
    void operator=(render_state&&) = delete;

    void clear_targets(const kl::float4& color);
};
