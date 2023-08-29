#include "editor.h"


void render_collider(editor_state* state)
{
    state->gpu->bind_raster_state(state->raster_states.wireframe);
    state->gpu->bind_depth_state(state->depth_states.disabled);
    state->gpu->bind_render_shaders(state->render_shaders.object_single);

    struct VS_DATA
    {
        kl::float4x4 wvp_matrix;
    } vs_data = {};

    vs_data.wvp_matrix = state->scene->camera->matrix() * state->scene->selected_entity->collider_matrix();
    state->render_shaders.object_single.vertex_shader.update_cbuffer(vs_data);

    struct PS_DATA
    {
        kl::float4 object_color; // (color.r, color.g, color.b, none)
    } ps_data = {};
    
    ps_data.object_color = state->gui_state->color_alternative;
    state->render_shaders.object_single.pixel_shader.update_cbuffer(ps_data);

    switch (state->scene->selected_entity->collider()->type()) {
    case PxGeometryType::Enum::eBOX:
        state->gpu->draw(state->default_mesh.cube->graphics_buffer);
        break;

    case PxGeometryType::Enum::eSPHERE:
        state->gpu->draw(state->default_mesh.sphere->graphics_buffer);
        break;

    case PxGeometryType::Enum::eCAPSULE:
        state->gpu->draw(state->default_mesh.capsule->graphics_buffer);
        break;

    case PxGeometryType::Enum::ePLANE:
        break;

    case PxGeometryType::Enum::eTRIANGLEMESH:
        auto& mesh = state->scene->selected_entity->mesh;
        if (mesh) { state->gpu->draw(mesh->graphics_buffer); }
        break;
    }
}
