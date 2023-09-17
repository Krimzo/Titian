export module render_collider;

export import render_chain;

export void render_collider(EditorState* state)
{
    state->gpu->bind_raster_state(state->raster_states.wireframe);
    state->gpu->bind_depth_state(state->depth_states.disabled);
    state->gpu->bind_render_shaders(state->render_shaders.object_single);

    class RenderColliderVS
    {
    public:
        kl::Float4x4 wvp_matrix;
    } vs_data = {};

    vs_data.wvp_matrix = state->scene->camera->matrix() * state->scene->selected_entity->collider_matrix();
    state->render_shaders.object_single.vertex_shader.update_cbuffer(vs_data);

    class RenderColliderPS
    {
    public:
        kl::Float4 object_color; // (color.r, color.g, color.b, none)
    } ps_data = {};
    
    ps_data.object_color = state->gui_state->color_alternative;
    state->render_shaders.object_single.pixel_shader.update_cbuffer(ps_data);

    switch (state->scene->selected_entity->collider()->type()) {
    case physx::PxGeometryType::Enum::eBOX:
        state->gpu->draw(state->default_mesh.cube->graphics_buffer);
        break;

    case physx::PxGeometryType::Enum::eSPHERE:
        state->gpu->draw(state->default_mesh.sphere->graphics_buffer);
        break;

    case physx::PxGeometryType::Enum::eCAPSULE:
        state->gpu->draw(state->default_mesh.capsule->graphics_buffer);
        break;

    case physx::PxGeometryType::Enum::ePLANE:
        break;

    case physx::PxGeometryType::Enum::eTRIANGLEMESH:
        auto& mesh = state->scene->selected_entity->mesh;
        if (mesh) { state->gpu->draw(mesh->graphics_buffer); }
        break;
    }
}
