export module gui_helper;

export import klib;

export namespace GUI::drag_drop {
    inline std::unordered_map<std::string, std::any> _data = {};

    template<typename T>
    void write_data(const std::string& id, const T& data, const kl::dx::shader_view& texture = nullptr)
    {
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(id.c_str(), nullptr, 0);
            if (texture) ImGui::Image(texture.Get(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            _data[id] = { data };
            ImGui::EndDragDropSource();
        }
    }

    template<typename T>
    std::optional<T> read_data(const std::string& id)
    {
        std::optional<T> result = {};
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::AcceptDragDropPayload(id.c_str())) {
                const std::any data = _data[id];
                result = std::any_cast<T>(data);
                _data.erase(id);
            }
            ImGui::EndDragDropTarget();
        }
        return result;
    }
}
