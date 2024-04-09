#pragma once

#include "gui/gui_section.h"
#include "application/app_layer.h"
#include "components/texture.h"
#include "utility/string_util.h"
#include "utility/file_util.h"


namespace titian {
	class GUISectionExplorer : public GUISection
	{
	public:
        AppLayer* app_layer = nullptr;
        GUILayer* gui_layer = nullptr;

        kl::Object<Texture> default_file_texture = nullptr;
        kl::Object<Texture> mesh_file_texture = nullptr;
        kl::Object<Texture> texture_file_texture = nullptr;
        kl::Object<Texture> script_file_texture = nullptr;
        kl::Object<Texture> shader_file_texture = nullptr;
        kl::Object<Texture> scene_file_texture = nullptr;
        kl::Object<Texture> default_dir_texture = nullptr;
        kl::Object<Texture> parent_dir_texture = nullptr;

        GUISectionExplorer(AppLayer* app_layer, GUILayer* gui_layer);

        void render_gui() override;

    private:
        std::string m_path = std::filesystem::absolute(".").string();
        float m_icon_size = 65.0f;

        void handle_file_entry(const std::filesystem::path& file);
        void handle_directory_entry(const std::filesystem::path& directory, bool is_parent_dir);

        void handle_item_transfer(const std::string& item, const std::string& destination);
        kl::dx::ShaderView file_icon(FileType type);
    };
}
