#pragma once

#include "gui/gui_section.h"
#include "application/app_layer.h"
#include "components/texture.h"
#include "utility/string_util.h"


namespace titian {
	class GUISectionExplorer : public GUISection
	{
	public:
        enum class FileType
        {
            DEFAULT = 0,
            MESH,
            TEXTURE,
            SCRIPT,
            SHADER,
            SCENE,
        };

        AppLayer* app_layer = nullptr;

        std::string path = std::filesystem::absolute(".").string();

        kl::Object<Texture> default_file_texture = nullptr;
        kl::Object<Texture> mesh_file_texture = nullptr;
        kl::Object<Texture> texture_file_texture = nullptr;
        kl::Object<Texture> script_file_texture = nullptr;
        kl::Object<Texture> shader_file_texture = nullptr;
        kl::Object<Texture> scene_file_texture = nullptr;

        kl::Object<Texture> default_dir_texture = nullptr;
        kl::Object<Texture> parent_dir_texture = nullptr;

        int icon_size = 65;

        GUISectionExplorer(AppLayer* app_layer);

        void render_gui() override;

    private:
        void handle_file_entry(const std::filesystem::path& file);
        void handle_directory_entry(const std::filesystem::path& directory, bool is_parent_dir);

        FileType classify_file(const std::filesystem::path& file);
        kl::dx::ShaderView file_icon(FileType type);

        void drag_file(const std::filesystem::path& file, FileType type, const kl::dx::ShaderView& texture);
	};
}
