#include "main.h"


titian::OutlinePass::OutlinePass(const LayerPackage& package)
    : RenderPass("OutlinePass", package)
{}

bool titian::OutlinePass::is_renderable() const
{
    return !editor_layer->selected_entities.empty();
}

titian::StatePackage titian::OutlinePass::get_state_package()
{
    RenderStates* render_states = &render_layer->states;

    StatePackage package = {};
    package.raster_state = render_states->raster_states->solid;
    package.depth_state = render_states->depth_states->disabled;
    package.shader_state = render_states->shader_states->outline_pass;
    return package;
}

void titian::OutlinePass::render_self(StatePackage& package)
{
    std::vector<float> entitiy_indices;
    for (uint32_t counter_id = 0; const auto& [name, _] : *game_layer->scene) {
        counter_id += 1;
        if (editor_layer->selected_entities.contains(name)) {
			entitiy_indices.push_back((float) counter_id);
		}
    }
    if (entitiy_indices.empty())
        return;

    kl::GPU* gpu = &app_layer->gpu;

    // Target
    gpu->bind_target_depth_views({ render_layer->render_texture->target_view }, render_layer->depth_texture->depth_view);

    load_selected_entities(entitiy_indices);
    gpu->bind_shader_view_for_pixel_shader(render_layer->picking_texture->shader_view, 0);
    gpu->bind_shader_view_for_pixel_shader(m_selected_entities_view, 1);

    struct PS_CB
    {
        kl::Float4 OUTLINE_COLOR;
        alignas(16) int32_t OUTLINE_SIZE;
        uint32_t SELECTED_COUNT;
    };

    const PS_CB ps_cb{
        .OUTLINE_COLOR = gui_layer->special_color,
        .OUTLINE_SIZE = editor_layer->outline_size,
		.SELECTED_COUNT = (uint32_t) entitiy_indices.size(),
    };
    package.shader_state.pixel_shader.update_cbuffer(ps_cb);

    gpu->draw(render_layer->screen_mesh);
    gpu->unbind_shader_view_for_pixel_shader(1);
    gpu->unbind_shader_view_for_pixel_shader(0);
}

void titian::OutlinePass::load_selected_entities(const std::vector<float>& entitiy_indices)
{
	kl::GPU* gpu = &app_layer->gpu;
    if (gpu->vertex_buffer_size(m_selected_entities_buff, sizeof(float)) < entitiy_indices.size()) {
        kl::dx::BufferDescriptor descriptor{};
        descriptor.Usage = D3D11_USAGE_DYNAMIC;
        descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        descriptor.StructureByteStride = sizeof(float);
        descriptor.ByteWidth = (UINT) entitiy_indices.size() * sizeof(float);

        kl::dx::SubresourceDescriptor subresource{};
        subresource.pSysMem = entitiy_indices.data();

        m_selected_entities_buff = gpu->create_buffer(&descriptor, &subresource);
        if (!m_selected_entities_buff) {
            return;
        }
        m_selected_entities_view = gpu->create_shader_view(m_selected_entities_buff, nullptr);
        if (!m_selected_entities_view) {
            return;
        }
    }
    else {
        gpu->write_to_buffer(m_selected_entities_buff, entitiy_indices.data(), entitiy_indices.size() * sizeof(float));
    }
}
