#include "main.h"


titian::GUISectionAnimationEditor::GUISectionAnimationEditor(const LayerPackage& package)
    : GUISection("GUISectionAnimationEditor", package)
{
    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;

    camera = new Camera(scene->physics(), true);
    render_texture = new Texture(gpu);
    depth_texture = new Texture(gpu);

    sun_direction = kl::normalize(kl::Float3(-0.5f, -0.75f, 1.0f));

    camera->background = kl::Color{ 30, 30, 30 };
    camera->set_position({ -0.34f, 0.18f, -0.94f });
    camera->speed = 3.1f;
}

void titian::GUISectionAnimationEditor::render_gui()
{
    const TimeBomb _ = this->time_it();

    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;
    Animation* animation = &scene->get_animation(this->selected_animation);

    if (imgui::Begin("Animation Editor")) {
        const float available_width = imgui::GetContentRegionAvail().x;
        imgui::Columns(2, "AnimationEditorColumns", false);

        imgui::SetColumnWidth(imgui::GetColumnIndex(), available_width * 0.25f);
        if (imgui::BeginChild("Animations")) {
            display_animations(gpu, scene);
        }
        imgui::EndChild();
        imgui::NextColumn();

        imgui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        imgui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        imgui::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        if (imgui::BeginChild("Animation View", {}, was_focused)) {
            const kl::Int2 viewport_size = { (int) imgui::GetContentRegionAvail().x, (int) imgui::GetContentRegionAvail().y };
            if (was_focused) {
                update_animation_camera();
            }
            if (animation) {
                render_selected_animation(gpu, animation, viewport_size);
                const kl::dx::ShaderView& shader_view = render_texture->shader_view;
                imgui::Image(render_texture->shader_view.Get(), { (float) viewport_size.x, (float) viewport_size.y });
            }
            was_focused = imgui::IsWindowFocused();
        }
        imgui::EndChild();

        imgui::PopStyleColor();
        imgui::PopStyleVar(2);

        show_animation_properties(animation);
    }
    imgui::End();
}

void titian::GUISectionAnimationEditor::display_animations(kl::GPU* gpu, Scene* scene)
{
    // New animation
    if (imgui::BeginPopupContextWindow("NewAnimation", ImGuiPopupFlags_MouseButtonMiddle)) {
        imgui::Text("New Animation");

        if (std::optional opt_name = gui_input_waited("##CreateAnimationInput", {})) {
            const std::string& name = opt_name.value();
            if (!name.empty() && !scene->animations.contains(name)) {
                kl::Object animation = new Animation(scene);
                scene->animations[name] = animation;
                imgui::CloseCurrentPopup();
            }
        }
        imgui::EndPopup();
    }

    // Animations
    const std::string filter = gui_input_continuous("Search###AnimationEditor");
    for (auto& [animation_name, animation] : scene->animations) {
        if (!filter.empty() && animation_name.find(filter) == -1) {
            continue;
        }

        if (imgui::Selectable(animation_name.c_str(), animation_name == this->selected_animation)) {
            this->selected_animation = animation_name;
        }

        if (imgui::BeginPopupContextItem(animation_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            imgui::Text("Edit Animation");

            if (std::optional opt_name = gui_input_waited("##RenameAnimationInput", animation_name)) {
                const std::string& name = opt_name.value();
                if (!name.empty() && !scene->meshes.contains(name)) {
                    if (this->selected_animation == animation_name) {
                        this->selected_animation = name;
                    }
                    scene->animations[name] = animation;
                    scene->animations.erase(animation_name);
                    should_break = true;
                    imgui::CloseCurrentPopup();
                }
            }

            if (imgui::Button("Delete", { -1.0f, 0.0f })) {
                if (this->selected_animation == animation_name) {
                    this->selected_animation = "/";
                }
                scene->animations.erase(animation_name);
                should_break = true;
                imgui::CloseCurrentPopup();
            }

            imgui::EndPopup();
            if (should_break) {
                break;
            }
        }
    }
}

void titian::GUISectionAnimationEditor::update_animation_camera()
{
    if (imgui::IsMouseClicked(ImGuiMouseButton_Right)) {
        initial_camera_info = camera_info;
    }

    const int scroll = app_layer->window.mouse.scroll();
    if (imgui::IsMouseDown(ImGuiMouseButton_Right)) {
        const ImVec2 drag_delta = imgui::GetMouseDragDelta(ImGuiMouseButton_Right);
        camera_info.x = initial_camera_info.x + drag_delta.x * camera->sensitivity;
        camera_info.y = initial_camera_info.y + drag_delta.y * camera->sensitivity;
        camera_info.y = kl::clamp(camera_info.y, -85.0f, 85.0f);

        camera->set_position({
            kl::sin_deg(camera_info.x),
            kl::tan_deg(camera_info.y),
            kl::cos_deg(camera_info.x),
        });

        camera->speed += (last_scroll - scroll) * 0.1f;
        camera->speed = std::max(camera->speed, 0.1f);
    }
    last_scroll = scroll;

    const float camera_distance = camera->speed;
    camera->set_position(kl::normalize(camera->position()) * camera_distance);
    camera->set_forward(-camera->position());
}

void titian::GUISectionAnimationEditor::render_selected_animation(kl::GPU* gpu, const Animation* animation, const kl::Int2 viewport_size)
{
    if (viewport_size.x <= 0 || viewport_size.y <= 0) {
        return;
    }

    if (render_texture->graphics_buffer_size() != viewport_size) {
        render_texture->graphics_buffer = gpu->create_target_texture(viewport_size);
        render_texture->create_target_view(nullptr);
        render_texture->create_shader_view(nullptr);
    }
    if (depth_texture->graphics_buffer_size() != viewport_size) {
        kl::dx::TextureDescriptor descriptor = {};
        descriptor.Width = viewport_size.x;
        descriptor.Height = viewport_size.y;
        descriptor.MipLevels = 1;
        descriptor.ArraySize = 1;
        descriptor.Format = DXGI_FORMAT_D32_FLOAT;
        descriptor.SampleDesc.Count = 1;
        descriptor.Usage = D3D11_USAGE_DEFAULT;
        descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depth_texture->graphics_buffer = gpu->create_texture(&descriptor, nullptr);
        depth_texture->create_depth_view(nullptr);
    }

    gpu->clear_target_view(render_texture->target_view, camera->background);
    gpu->clear_depth_view(depth_texture->depth_view, 1.0f, 0xFF);

    Mesh* mesh = nullptr;
    if (m_animating) {
        m_frame_index = animation->get_index(m_timer.elapsed());
    }
    if (m_frame_index >= 0 && m_frame_index < (int) animation->meshes.size()) {
        mesh = game_layer->scene->helper_get_mesh(animation->meshes[m_frame_index]);
    }
    if (!mesh) {
        return;
    }

    gpu->bind_target_depth_view(render_texture->target_view, depth_texture->depth_view);

    const kl::Int2 old_viewport_size = gpu->viewport_size();
    gpu->set_viewport_size(viewport_size);

    RenderStates* states = &render_layer->states;
    gpu->bind_raster_state(mesh->render_wireframe ? states->raster_states->wireframe : states->raster_states->solid);
    gpu->bind_depth_state(states->depth_states->enabled);

    kl::RenderShaders& render_shaders = states->shader_states->solid_lit_pass;
    gpu->bind_render_shaders(render_shaders);

    camera->update_aspect_ratio(viewport_size);

    struct VS_CB
    {
        kl::Float4x4 W;
        alignas(16) kl::Float4x4 WVP;
    };

    const VS_CB vs_cb{
        .W = {},
        .WVP = camera->camera_matrix(),
    };

    struct PS_CB
    {
        kl::Float4 OBJECT_COLOR;
        alignas(16) kl::Float3 SUN_DIRECTION;
    };

    const PS_CB ps_cb{
        .OBJECT_COLOR = line_color,
        .SUN_DIRECTION = sun_direction,
    };

    render_shaders.vertex_shader.update_cbuffer(vs_cb);
    render_shaders.pixel_shader.update_cbuffer(ps_cb);

    gpu->draw(mesh->graphics_buffer, mesh->casted_topology());

    gpu->bind_internal_views();
    gpu->set_viewport_size(old_viewport_size);
}

void titian::GUISectionAnimationEditor::show_animation_properties(Animation* animation)
{
    kl::Window* window = &app_layer->window;

    const int current_scroll = window->mouse.scroll();

    if (imgui::Begin("Animation Properties") && animation) {
        if (imgui::IsWindowFocused()) {
            m_start_mesh_index += m_last_scroll - current_scroll;
            m_start_mesh_index = std::clamp(m_start_mesh_index, 0, std::max<int>((int) animation->meshes.size() - 1, 0));
        }

        /*-*/
        imgui::Text("Animation Editor");
        if (imgui::DragFloat3("Sun Direction", sun_direction, 0.01f)) {
            sun_direction = kl::normalize(sun_direction);
        }
        imgui::SliderInt("Frame", &m_frame_index, 0, std::max<int>((int) animation->meshes.size() - 1, 0));
        if (imgui::Checkbox("Animating?", &m_animating)) {
            if (m_animating) {
                m_timer.restart();
            }
            else {
                m_timer.stop();
            }
        }

        /*-*/
        imgui::Text("Info");
        imgui::Text("Name: ");
        imgui::SameLine();
        gui_colored_text(selected_animation, gui_layer->special_color);

        imgui::DragFloat("FPS", &animation->fps, 1.0f, 0.0f, 10'000.0f);

        int mesh_count = (int) animation->meshes.size();
        if (imgui::DragInt("Mesh Count", &mesh_count, 1.0f, 0, 10'000)) {
            animation->meshes.resize(mesh_count);
        }

        for (int i = m_start_mesh_index; i < (m_start_mesh_index + 10) && i < (int) animation->meshes.size(); i++) {
            if (imgui::BeginCombo(kl::format(i, ". Mesh").c_str(), animation->meshes[i].c_str())) {
                for (auto& [name, _] : game_layer->scene->meshes) {
                    if (imgui::Selectable(name.c_str(), name == animation->meshes[i])) {
                        animation->meshes[i] = name;
                    }
                }
                imgui::EndCombo();
            }
        }

    }
    imgui::End();

    m_last_scroll = current_scroll;
}
