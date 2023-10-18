export module gui_helper;

export import klib;

std::unordered_map<std::string, std::any> DATA = {};

export namespace titian {
    template<typename T>
    void gui_set_drag_drop(const std::string& id, const T& data, const kl::dx::ShaderView& texture = nullptr)
    {
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(id.c_str(), nullptr, 0);
            if (texture) {
                ImGui::Image(texture.Get(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            }
            DATA[id] = std::any{ data };
            ImGui::EndDragDropSource();
        }
    }

    template<typename T>
    std::optional<T> gui_get_drag_drop(const std::string& id)
    {
        std::optional<T> result = {};
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::AcceptDragDropPayload(id.c_str())) {
                const std::any data = DATA[id];
                result = std::any_cast<T>(data);
                DATA.erase(id);
            }
            ImGui::EndDragDropTarget();
        }
        return result;
    }

    void gui_colored_text(const kl::Float4& color, const std::string_view& message)
    {
        ImGui::TextColored(reinterpret_cast<const ImVec4&>(color), message.data());
    }
}
