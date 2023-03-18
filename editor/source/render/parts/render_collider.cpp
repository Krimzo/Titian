#include "render/render_chain.h"

#include "cbuffers/collider_render.h"


void render_collider(editor_state* state)
{
    auto collider = state->scene->selected_entity->get_collider();
    if (!collider) { return; }

    state->gpu->bind_raster_state(state->raster_states["collider"]);
    state->gpu->bind_depth_state(state->depth_states["collider"]);

    state->gpu->bind_render_shaders(state->shaders["collider"]);

    collider_render_vs_cb vs_cb = {};
    vs_cb.wvp_matrix = state->scene->camera->matrix() * state->scene->selected_entity->collider_matrix();

    state->gpu->bind_cb_for_vertex_shader(collider_render_vs_const_buffer, 0);
    state->gpu->set_cb_data(collider_render_vs_const_buffer, vs_cb);

    collider_render_ps_cb ps_cb = {};
    ps_cb.object_color = state->gui_state.collider_color;

    state->gpu->bind_cb_for_pixel_shader(collider_render_ps_const_buffer, 0);
    state->gpu->set_cb_data(collider_render_ps_const_buffer, ps_cb);

    switch (collider->get_type()) {
    case PxGeometryType::Enum::eBOX:
        state->gpu->draw_mesh(state->default_meshes["cube"]->graphics_buffer);
        break;
    case PxGeometryType::Enum::eSPHERE:
        state->gpu->draw_mesh(state->default_meshes["sphere"]->graphics_buffer);
        break;
    case PxGeometryType::Enum::eCAPSULE:
        state->gpu->draw_mesh(state->default_meshes["capsule"]->graphics_buffer);
        break;

    case PxGeometryType::Enum::ePLANE:

        break;
    case PxGeometryType::Enum::eTRIANGLEMESH:
        auto& mesh = state->scene->selected_entity->mesh;
        if (mesh) { state->gpu->draw_mesh(mesh->graphics_buffer); }
        break;
    }
}
