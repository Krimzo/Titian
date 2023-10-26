export module gui_helper;

export import klib;

std::unordered_map<std::string, std::any> DRAG_DROP_DATA = {};
std::unordered_map<std::string, char[101]> INPUT_CONTINUOUS_DATA = {};
std::unordered_map<std::string, char[101]> INPUT_WAITED_DATA = {};

export namespace titian {
    inline void gui_colored_text(const std::string_view& message, const kl::Float4& color)
    {
        ImGui::TextColored(reinterpret_cast<const ImVec4&>(color), message.data());
    }

    inline std::string gui_input_continuous(const std::string& id)
    {
        auto& buffer = INPUT_CONTINUOUS_DATA[id];
        const int buffer_capacity = static_cast<int>(std::size(buffer) - 1);
        ImGui::InputText(id.c_str(), buffer, buffer_capacity);
        return { { buffer } };
    }

    inline std::optional<std::string> gui_input_waited(const std::string& id, const std::string_view& to_copy)
    {
        auto& buffer = INPUT_WAITED_DATA[id];
        const int buffer_capacity = static_cast<int>(std::size(buffer) - 1);
        if (!to_copy.empty()) {
            memcpy(buffer, to_copy.data(), std::min(buffer_capacity, static_cast<int>(to_copy.size())));
        }
        if (ImGui::InputText(id.c_str(), buffer, buffer_capacity, ImGuiInputTextFlags_EnterReturnsTrue)) {
            const std::string result = { buffer };
            memset(buffer, 0, std::size(buffer));
            return { result };
        }
        return {};
    }

    template<typename T>
    void gui_set_drag_drop(const std::string& id, const T& data, const kl::dx::ShaderView& texture = nullptr)
    {
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(id.c_str(), nullptr, 0);
            if (texture) {
                ImGui::Image(texture.Get(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            }
            DRAG_DROP_DATA[id] = std::any{ data };
            ImGui::EndDragDropSource();
        }
    }

    template<typename T>
    std::optional<T> gui_get_drag_drop(const std::string& id)
    {
        std::optional<T> result = {};
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::AcceptDragDropPayload(id.c_str())) {
                const std::any data = DRAG_DROP_DATA[id];
                result = std::any_cast<T>(data);
                DRAG_DROP_DATA.erase(id);
            }
            ImGui::EndDragDropTarget();
        }
        return result;
    }
}
