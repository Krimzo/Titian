#include "main.h"


titian::DisplayPass::DisplayPass(const LayerPackage& package)
    : RenderPass("DisplayPass", package)
{}

bool titian::DisplayPass::is_renderable() const
{
    return true;
}

titian::StatePackage titian::DisplayPass::get_state_package()
{
    RenderStates* render_states = &render_layer->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->solid;
    package.depth_state = render_states->depth_states->disabled;
    package.shader_state = render_states->shader_states->display_pass;
    return package;
}

void titian::DisplayPass::render_self(StatePackage& package)
{
    kl::Window* window = &app_layer->window;
    kl::GPU* gpu = &app_layer->gpu;

    const Int2 window_size = window->size();
    render_layer->resize(window_size);

    // Target
    gpu->bind_internal_views();
    gpu->set_viewport_size(window_size);

    gpu->bind_shader_view_for_pixel_shader(render_layer->screen_texture->shader_view, 0);
    gpu->draw(render_layer->screen_mesh);
}
