export module section_explorer;

export import gui_section;
export import app_layer;
export import texture;

export namespace titian {
	class GUISectionExplorer : public GUISection
	{
	public:
        enum class FileType
        {
            DEFAULT = 0,
            MESH,
            TEXTURE,
            SCRIPT,
            SCENE,
        };

        AppLayer* app_layer = nullptr;

        std::string path = std::filesystem::absolute(".").string();

        kl::Object<Texture> default_file_texture = nullptr;
        kl::Object<Texture> mesh_file_texture = nullptr;
        kl::Object<Texture> texture_file_texture = nullptr;
        kl::Object<Texture> script_file_texture = nullptr;
        kl::Object<Texture> scene_file_texture = nullptr;

        kl::Object<Texture> default_dir_texture = nullptr;
        kl::Object<Texture> parent_dir_texture = nullptr;

        int icon_size = 65;

		GUISectionExplorer(AppLayer* app_layer)
		{
            this->app_layer = app_layer;
            kl::GPU* gpu = &app_layer->gpu;

            default_file_texture = new Texture(gpu);
            mesh_file_texture = new Texture(gpu);
            texture_file_texture = new Texture(gpu);
            script_file_texture = new Texture(gpu);
            scene_file_texture = new Texture(gpu);

            default_dir_texture = new Texture(gpu);
            parent_dir_texture = new Texture(gpu);

            default_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/default_file.png"));
            mesh_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/mesh_file.png"));
            texture_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/texture_file.png"));
            script_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/script_file.png"));
            scene_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/scene_file.png"));

            default_dir_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/default_dir.png"));
            parent_dir_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/parent_dir.png"));

            default_file_texture->create_shader_view(nullptr);
            mesh_file_texture->create_shader_view(nullptr);
            texture_file_texture->create_shader_view(nullptr);
            script_file_texture->create_shader_view(nullptr);
            scene_file_texture->create_shader_view(nullptr);

            default_dir_texture->create_shader_view(nullptr);
            parent_dir_texture->create_shader_view(nullptr);

            kl::assert(default_file_texture->shader_view, "Failed to init DEFAULT file texture");
            kl::assert(mesh_file_texture->shader_view, "Failed to init MESH file texture");
            kl::assert(texture_file_texture->shader_view, "Failed to init TEXTURE file texture");
            kl::assert(script_file_texture->shader_view, "Failed to init SCRIPT file texture");
            kl::assert(scene_file_texture->shader_view, "Failed to init SCENE file texture");

            kl::assert(default_dir_texture->shader_view, "Failed to init DEFAULT dir texture");
            kl::assert(parent_dir_texture->shader_view, "Failed to init PARENT dir texture");
        }

		~GUISectionExplorer() override
		{}

		void render_gui() override
		{
            std::list<std::filesystem::path> directories = {};
            std::list<std::filesystem::path> files = {};

            try {
                for (auto& entry : std::filesystem::directory_iterator(path)) {
                    (entry.is_directory() ? directories : files).push_back(entry);
                }
            }
            catch (std::exception ignored) {
            }

            if (ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
                const float window_width = ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.0f;
                int column_count = static_cast<int>(window_width / (icon_size + ImGui::GetStyle().CellPadding.x * 2.0f));
                if (column_count < 1) {
                    column_count = 1;
                }

                ImGui::Text(path.c_str());
                ImGui::Separator();

                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 0.0f, 4.0f });

                if (ImGui::BeginTable("##ExplorerTable", (int) column_count)) {
                    const std::filesystem::path current_path = { path };
                    if (current_path.has_parent_path()) {
                        ImGui::TableNextColumn();
                        handle_directory_entry(current_path.parent_path(), true);
                    }

                    for (auto& dir : directories) {
                        ImGui::TableNextColumn();
                        handle_directory_entry(dir, false);
                    }

                    for (auto& file : files) {
                        ImGui::TableNextColumn();
                        handle_file_entry(file);
                    }

                    ImGui::EndTable();
                }

                ImGui::PopStyleVar();

                if (ImGui::BeginPopupContextWindow()) {
                    ImGui::SliderInt("Icon Size", &icon_size, 25, 250);
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
		}

    private:
        void handle_file_entry(const std::filesystem::path& file)
        {
            const std::string path = std::filesystem::absolute(file).string();
            const FileType file_type = classify_file(file);
            const kl::dx::ShaderView icon = file_icon(file_type);

            constexpr float padding = 5.0f;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

            const float icon_size = static_cast<float>(this->icon_size);
            const float text_height = ImGui::CalcTextSize(path.c_str()).y;

            if (ImGui::BeginChild(path.c_str(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
                const ImVec2 cursor_pos = ImGui::GetCursorPos();
                if (ImGui::ImageButton(path.c_str(), icon.Get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
                    ShellExecuteA(nullptr, nullptr, path.c_str(), nullptr, nullptr, 5);
                }
                drag_file(path, file_type, icon);

                ImGui::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
                ImGui::Text(file.filename().string().c_str());
            }
            ImGui::EndChild();

            ImGui::PopStyleVar(2);

            if (ImGui::BeginPopupContextItem()) {
                std::error_code error = {};
                if (std::optional new_name = gui_input_waited("##RenameFileInput", file.filename().string())) {
                    std::filesystem::path new_file = file;
                    new_file.replace_filename(new_name.value());

                    std::filesystem::rename(file, new_file, error);
                    if (error) {
                        Logger::log("Failed to rename file ", file, " to ", new_file);
                    }
                    else {
                        Logger::log("Renamed file ", file, " to ", new_file);
                    }
                }
                ImGui::Text(kl::format(std::setprecision(2), std::filesystem::file_size(file, error) / 1048576.0f, " mb").c_str());

                if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                    std::filesystem::remove(file, error);
                    if (error) {
                        Logger::log("Failed to delete file ", file);
                    }              
                    else {         
                        Logger::log("Deleted file ", file);
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        void handle_directory_entry(const std::filesystem::path& directory, const bool is_parent_dir)
        {
            const std::string path = std::filesystem::absolute(directory).string();
            const kl::dx::ShaderView icon = is_parent_dir ? parent_dir_texture->shader_view : default_dir_texture->shader_view;
            std::error_code ignored_error = {};

            constexpr float padding = 5.0f;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

            const float icon_size = static_cast<float>(this->icon_size);
            const float text_height = ImGui::CalcTextSize(path.c_str()).y;

            constexpr float shadow_size = 2.0f;
            const ImVec2 child_size = { icon_size + padding * 2, icon_size + text_height + padding * 4.0f };

            if (ImGui::BeginChild(kl::format(path, "_elder").c_str(), { child_size.x + shadow_size, child_size.y + shadow_size })) {
                if (ImGui::BeginChild(path.c_str(), child_size, true, ImGuiWindowFlags_NoScrollbar)) {
                    const ImVec2 cursor_pos = ImGui::GetCursorPos();
                    if (ImGui::ImageButton(path.c_str(), icon.Get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
                        this->path = path;
                    }

                    ImGui::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
                    ImGui::Text(directory.filename().string().c_str());
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();

            ImGui::PopStyleVar(2);
        }

        FileType classify_file(const std::filesystem::path& file)
        {
            const std::string extension = file.extension().string();
            if (extension == ".obj") {
                return FileType::MESH;
            }
            if (extension == ".jpg" || extension == ".png") {
                return FileType::TEXTURE;
            }
            if (extension == ".titian") {
                return FileType::SCENE;
            }
            if (extension == ".dll" || extension == ".chai") {
                return FileType::SCRIPT;
            }
            return FileType::DEFAULT;
        }

        kl::dx::ShaderView file_icon(const FileType type)
        {
            switch (type)
            {
            case FileType::MESH:    return mesh_file_texture->shader_view;
            case FileType::TEXTURE: return texture_file_texture->shader_view;
            case FileType::SCRIPT:  return script_file_texture->shader_view;
            case FileType::SCENE:   return scene_file_texture->shader_view;
            }
            return default_file_texture->shader_view;
        }

        void drag_file(const std::filesystem::path& file, const FileType type, const kl::dx::ShaderView& texture)
        {
            const std::string path = file.string();
            switch (type)
            {
            case FileType::MESH:
                gui_set_drag_drop<std::string>("MeshFile", path, texture);
                break;
            case FileType::TEXTURE:
                gui_set_drag_drop<std::string>("TextureFile", path, texture);
                break;
            case FileType::SCRIPT:
                gui_set_drag_drop<std::string>("ScriptFile", path, texture);
                break;
            case FileType::SCENE:
                gui_set_drag_drop<std::string>("SceneFile", path, texture);
                break;
            }
        }
	};
}
