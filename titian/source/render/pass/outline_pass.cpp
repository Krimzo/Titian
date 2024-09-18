#include "titian.h"


titian::OutlinePass::OutlinePass()
    : RenderPass("OutlinePass")
{}

void titian::OutlinePass::state_package(StatePackage* package)
{
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    package->raster_state = render_layer->raster_states->solid;
    package->depth_state = render_layer->depth_states->disabled;
    package->shader_state = render_layer->shader_states->outline_pass;
}

void titian::OutlinePass::render_self(StatePackage* package)
{
	EditorLayer* editor_layer = Layers::get<EditorLayer>();
	GameLayer* game_layer = Layers::get<GameLayer>();
	RenderLayer* render_layer = Layers::get<RenderLayer>();
    GUILayer* gui_layer = Layers::get<GUILayer>();
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

    if (editor_layer->selected_entities.empty())
        return;

    Vector<float> entitiy_indices;
    for (uint32_t counter = 0; const auto& [name, _] : game_layer->scene->entities()) {
        counter += 1;
        if (editor_layer->selected_entities.contains(name)) {
			entitiy_indices.push_back(float(counter));
		}
    }
    if (entitiy_indices.empty())
        return;

    load_selected_entities(entitiy_indices);
    gpu->bind_target_depth_view(package->camera->screen_texture->target_view, package->camera->depth_texture->depth_view);
    gpu->bind_shader_view_for_pixel_shader(package->camera->index_texture->shader_view, 0);
    gpu->bind_shader_view_for_pixel_shader(m_selected_entities_view, 1);

    struct alignas(16) CB
    {
        Float4 OUTLINE_COLOR;
        int OUTLINE_SIZE;
        int SELECTED_COUNT;
    };

    CB cb{};
    cb.OUTLINE_COLOR = gui_layer->special_color;
    cb.OUTLINE_SIZE = editor_layer->outline_size;
    cb.SELECTED_COUNT = (int) entitiy_indices.size();
    package->shader_state.upload(cb);

    gpu->draw(render_layer->screen_mesh);
    bench_add_draw_call();

    gpu->unbind_shader_view_for_pixel_shader(1);
    gpu->unbind_shader_view_for_pixel_shader(0);
    gpu->unbind_target_depth_views();
}

void titian::OutlinePass::load_selected_entities(const Vector<float>& entitiy_indices)
{
	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    if (gpu->vertex_buffer_size(m_selected_entities_buff, sizeof(float)) < entitiy_indices.size()) {
        dx::BufferDescriptor descriptor{};
        descriptor.Usage = D3D11_USAGE_DYNAMIC;
        descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        descriptor.StructureByteStride = sizeof(float);
        descriptor.ByteWidth = (UINT) entitiy_indices.size() * sizeof(float);
        dx::SubresourceDescriptor subresource{};
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
