export module section_explorer;

export import gui_section;
export import application_layer;
export import texture;
export import gui_helper;

export namespace titian {
	class GUISectionExplorer : public GUISection
	{
	public:
        enum class FileType
        {
            OTHER = 0,
            CODE,
            SCRIPT,
            MESH,
            TEXTURE,
            SCENE,
        };

        kl::Object<ApplicationLayer> app_layer = nullptr;

        std::string path = std::filesystem::absolute(".").string();

        kl::Object<Texture> code_file_texture = nullptr;
        kl::Object<Texture> script_file_texture = nullptr;
        kl::Object<Texture> mesh_file_texture = nullptr;
        kl::Object<Texture> texture_file_texture = nullptr;
        kl::Object<Texture> scene_file_texture = nullptr;
        kl::Object<Texture> default_file_texture = nullptr;

        kl::Object<Texture> empty_dir_texture = nullptr;
        kl::Object<Texture> parent_dir_texture = nullptr;
        kl::Object<Texture> default_dir_texture = nullptr;

        int icon_size = 65;

		GUISectionExplorer(kl::Object<ApplicationLayer>& app_layer)
		{
            this->app_layer = app_layer;

            kl::Object<kl::GPU> gpu = app_layer->gpu;

            code_file_texture = new Texture(gpu);
            script_file_texture = new Texture(gpu);
            mesh_file_texture = new Texture(gpu);
            texture_file_texture = new Texture(gpu);
            scene_file_texture = new Texture(gpu);
            default_file_texture = new Texture(gpu);

            empty_dir_texture = new Texture(gpu);
            parent_dir_texture = new Texture(gpu);
            default_dir_texture = new Texture(gpu);

            code_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/code_file.png"));
            script_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/script_file.png"));
            mesh_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/mesh_file.png"));
            texture_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/texture_file.png"));
            scene_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/scene_file.png"));
            default_file_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/default_file.png"));

            empty_dir_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/empty_dir.png"));
            parent_dir_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/parent_dir.png"));
            default_dir_texture->graphics_buffer = gpu->create_texture(kl::Image("builtin/textures/default_dir.png"));

            code_file_texture->create_shader_view(nullptr);
            script_file_texture->create_shader_view(nullptr);
            mesh_file_texture->create_shader_view(nullptr);
            texture_file_texture->create_shader_view(nullptr);
            scene_file_texture->create_shader_view(nullptr);
            default_file_texture->create_shader_view(nullptr);

            empty_dir_texture->create_shader_view(nullptr);
            parent_dir_texture->create_shader_view(nullptr);
            default_dir_texture->create_shader_view(nullptr);

            kl::assert(code_file_texture->shader_view, "Failed to init CODE file texture");
            kl::assert(script_file_texture->shader_view, "Failed to init SCRIPT file texture");
            kl::assert(mesh_file_texture->shader_view, "Failed to init MESH file texture");
            kl::assert(texture_file_texture->shader_view, "Failed to init TEXTURE file texture");
            kl::assert(scene_file_texture->shader_view, "Failed to init SCENE file texture");
            kl::assert(default_file_texture->shader_view, "Failed to init DEFAULT file texture");

            kl::assert(empty_dir_texture->shader_view, "Failed to init EMPTY dir texture");
            kl::assert(parent_dir_texture->shader_view, "Failed to init PARENT dir texture");
            kl::assert(default_dir_texture->shader_view, "Failed to init DEFAULT dir texture");
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
                int column_count = (int) (window_width / (icon_size + ImGui::GetStyle().CellPadding.x * 2.0f));
                if (column_count < 1) column_count = 1;

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

            const float icon_size = (float) this->icon_size;
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
        }

        void handle_directory_entry(const std::filesystem::path& directory, const bool is_parent_dir)
        {
            const std::string path = std::filesystem::absolute(directory).string();
            kl::dx::ShaderView icon = default_dir_texture->shader_view;
            std::error_code ignored_error = {};
            if (is_parent_dir) {
                icon = parent_dir_texture->shader_view;
            }
            else if (std::filesystem::is_empty(directory, ignored_error)) {
                icon = empty_dir_texture->shader_view;
            }

            constexpr float padding = 5.0f;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

            const float icon_size = (float) this->icon_size;
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
            if (extension == ".h" || extension == ".cpp") {
                return FileType::CODE;
            }
            if (extension == ".dll" || extension == ".class") {
                return FileType::SCRIPT;
            }
            if (extension == ".obj") {
                return FileType::MESH;
            }
            if (extension == ".jpg" || extension == ".png") {
                return FileType::TEXTURE;
            }
            if (extension == ".titian") {
                return FileType::SCENE;
            }
            return FileType::OTHER;
        }

        kl::dx::ShaderView file_icon(const FileType type)
        {
            switch (type) {
            case FileType::CODE:
                return code_file_texture->shader_view;
            case FileType::SCRIPT:
                return script_file_texture->shader_view;
            case FileType::MESH:
                return mesh_file_texture->shader_view;
            case FileType::TEXTURE:
                return texture_file_texture->shader_view;
            case FileType::SCENE:
                return scene_file_texture->shader_view;
            }
            return default_file_texture->shader_view;
        }

        void drag_file(const std::filesystem::path& file, const FileType type, const kl::dx::ShaderView& texture)
        {
            const std::string path = file.string();
            switch (type) {
            case FileType::CODE:
                gui_set_drag_drop("CodeFile", path, texture);
                break;
            case FileType::SCRIPT:
                gui_set_drag_drop("ScriptFile", path, texture);
                break;
            case FileType::MESH:
                gui_set_drag_drop("MeshFile", path, texture);
                break;
            case FileType::TEXTURE:
                gui_set_drag_drop("TextureFile", path, texture);
                break;
            case FileType::SCENE:
                gui_set_drag_drop("SceneFile", path, texture);
                break;
            }
        }
	};
}
