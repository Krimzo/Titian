#include "titian.h"


titian::GUISectionHelper::GUISectionHelper()
    : GUISection("GUISectionHelper")
{
    const auto create_texture = [&](Texture& texture, str filename)
    {
        texture.image.load_from_file(filename);
        texture.reload_as_2D();
        texture.create_shader_view();
        kl::assert(texture.shader_view, "Failed to init texture: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_texture(basic_entity_texture, "package/textures/scene_file.png"); });
    queue.add_task([&] { create_texture(animation_entity_texture, "package/textures/mesh_file.png"); });
    queue.finalize();
}

void titian::GUISectionHelper::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    GUILayer& gui_layer = Layers::get<GUILayer>();
    Scene& scene = Layers::get<GameLayer>().scene();

    if (im::Begin("Helper", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        const float icon_size = m_icon_size * gui_layer.dpi_scaling;
        const float window_width = im::GetWindowWidth() - im::GetStyle().WindowPadding.x * 2.0f;
        const float icon_width = icon_size + im::GetStyle().CellPadding.x * 2.0f;
        const int column_count = kl::max((int) (window_width / icon_width), 1);

        im::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 0.0f, 4.0f });

        if (im::BeginTable("##HelperTable", column_count)) {
            for (const auto& type : { DRAG_ENTITY_ENTITY, DRAG_ENTITY_CAMERA, DRAG_ENTITY_AMBIENT, DRAG_ENTITY_DIRECTIONAL }) {
                im::TableNextColumn();
                handle_basic_entry(type);
            }
            for (const auto& [name, _] : scene.animations) {
                im::TableNextColumn();
                handle_animation_entry(name);
            }
            im::EndTable();
        }

        im::PopStyleVar();

        if (im::IsKeyDown(ImGuiKey_LeftShift) && im::BeginPopupContextWindow()) {
            im::SliderFloat("Icon Size", &m_icon_size, 25.0f, 250.0f);
            im::EndPopup();
        }
    }
    im::End();
}

void titian::GUISectionHelper::handle_basic_entry(const StringView& name) const
{
    constexpr float padding = 5.0f;
    im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

    const dx::ShaderView icon = basic_entity_texture.shader_view;
    const float icon_size = m_icon_size * Layers::get<GUILayer>().dpi_scaling;
    const float text_height = im::CalcTextSize(name.data()).y;

    if (im::BeginChild(name.data(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
        const ImVec2 cursor_pos = im::GetCursorPos();
        im::ImageButton(name.data(), icon.get(), { icon_size, icon_size });
        gui_set_drag_drop<String>(DRAG_BASIC_ENTITY_ID, String(name), icon);
        im::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
        im::Text(name.data());
    }
    im::EndChild();

    im::PopStyleVar(2);
}

void titian::GUISectionHelper::handle_animation_entry(const StringView& name) const
{
    constexpr float padding = 5.0f;
    im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

    const dx::ShaderView icon = animation_entity_texture.shader_view;
    const float icon_size = m_icon_size * Layers::get<GUILayer>().dpi_scaling;
    const float text_height = im::CalcTextSize(name.data()).y;

    if (im::BeginChild(name.data(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
        const ImVec2 cursor_pos = im::GetCursorPos();
        im::ImageButton(name.data(), icon.get(), { icon_size, icon_size });
        gui_set_drag_drop<String>(DRAG_ANIMATION_ENTITY_ID, String(name), icon);
        im::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
        im::Text(name.data());
    }
    im::EndChild();

    im::PopStyleVar(2);
}
