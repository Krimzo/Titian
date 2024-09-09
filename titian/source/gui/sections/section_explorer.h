#pragma once

#include "gui/gui_section.h"
#include "application/app_layer.h"
#include "components/texture.h"
#include "utility/string_util.h"
#include "utility/file_util.h"


namespace titian {
    struct GUISectionExplorer : GUISection
	{
        Ref<Texture> default_file_texture = nullptr;
        Ref<Texture> mesh_file_texture = nullptr;
        Ref<Texture> texture_file_texture = nullptr;
        Ref<Texture> script_file_texture = nullptr;
        Ref<Texture> shader_file_texture = nullptr;
        Ref<Texture> scene_file_texture = nullptr;
        Ref<Texture> default_dir_texture = nullptr;
        Ref<Texture> parent_dir_texture = nullptr;

        GUISectionExplorer();

        void render_gui() override;

    private:
        String m_path = fs::absolute(".").string();
        float m_icon_size = 65.0f;

        void handle_file_entry(const fs::path& file);
        void handle_directory_entry(const fs::path& directory, bool is_parent_dir);

        void handle_item_transfer(const StringView& item, const StringView& destination);
        dx::ShaderView file_icon(FileType type);
    };
}
