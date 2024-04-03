#include "main.h"


titian::OutlinePass::OutlinePass(GameLayer* game_layer, EditorLayer* editor_layer, GUILayer* gui_layer)
    : RenderPass("OutlinePass", game_layer)
    , editor_layer(editor_layer)
    , gui_layer(gui_layer)
{}

bool titian::OutlinePass::is_renderable() const
{
    const Scene* scene = &game_layer->scene;
    return scene->get_entity(editor_layer->selected_entity);
}

titian::StatePackage titian::OutlinePass::get_state_package()
{
    RenderStates* render_states = &gui_layer->render_layer->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->solid;
    package.depth_state = render_states->depth_states->disabled;
    package.shader_state = render_states->shader_states->outline_pass;
    return package;
}

void titian::OutlinePass::render_self(StatePackage& package)
{
    RenderLayer* render_layer = gui_layer->render_layer;
    kl::GPU* gpu = &game_layer->app_layer->gpu;
    Scene* scene = &game_layer->scene;

    Entity* entity = &scene->get_entity(editor_layer->selected_entity);
    if (!entity) { return; }

    uint32_t counter_id = 0;
    for (auto& [_, ent] : *editor_layer->game_layer->scene) {
        counter_id += 1;
        if (&ent == entity) {
            break;
        }
    }

    gpu->bind_target_depth_views({ render_layer->render_texture->target_view }, render_layer->depth_texture->depth_view);
    gpu->bind_shader_view_for_pixel_shader(render_layer->picking_texture->shader_view, 0);

    struct PS_CB
    {
        float SELECTED_INDEX;
        kl::Float3 OUTLINE_COLOR;
    };

	const PS_CB ps_cb{
        .SELECTED_INDEX = static_cast<float>(counter_id),
        .OUTLINE_COLOR = gui_layer->special_color.xyz(),
    };
    package.shader_state.pixel_shader.update_cbuffer(ps_cb);

    gpu->draw(render_layer->screen_mesh);
}
