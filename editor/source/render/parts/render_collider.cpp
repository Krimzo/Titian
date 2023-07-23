#include "render/render_chain.h"

#include "cbuffers/collider_render.h"


void render_collider(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states.wireframe);
    state->gpu->bind_depth_state(state->depth_states.disabled);
    state->gpu->bind_render_shaders(state->render_shaders.object_solid);

    collider_render_vs_cb vs_cb = {};
    vs_cb.wvp_matrix = state->scene->camera->matrix() * state->scene->selected_entity->collider_matrix();
    state->render_shaders.object_solid.vertex_shader.update_cbuffer(vs_cb);

    collider_render_ps_cb ps_cb = {};
    ps_cb.object_color = state->gui_state->collider_color;
    state->render_shaders.object_solid.pixel_shader.update_cbuffer(ps_cb);

    switch (state->scene->selected_entity->get_collider()->get_type()) {
    case PxGeometryType::Enum::eBOX:
        state->gpu->draw(state->default_meshes["cube"]->graphics_buffer);
        break;

    case PxGeometryType::Enum::eSPHERE:
        state->gpu->draw(state->default_meshes["sphere"]->graphics_buffer);
        break;

    case PxGeometryType::Enum::eCAPSULE:
        state->gpu->draw(state->default_meshes["capsule"]->graphics_buffer);
        break;

    case PxGeometryType::Enum::ePLANE:
        break;

    case PxGeometryType::Enum::eTRIANGLEMESH:
        auto& mesh = state->scene->selected_entity->mesh;
        if (mesh) { state->gpu->draw(mesh->graphics_buffer); }
        break;
    }
}
