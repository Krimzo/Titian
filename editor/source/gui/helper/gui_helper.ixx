export module gui_helper;

export import klib;

static std::unordered_map<std::string, std::any> DATA = {};

export namespace gui::drag_drop {
    template<typename T>
    void write_data(const std::string& id, const T& data, const kl::dx::ShaderView& texture = nullptr)
    {
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(id.c_str(), nullptr, 0);
            if (texture) ImGui::Image(texture.Get(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            DATA[id] = { data };
            ImGui::EndDragDropSource();
        }
    }

    template<typename T>
    std::optional<T> read_data(const std::string& id)
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
}
