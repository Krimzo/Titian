#pragma once

#include "app/app_layer.h"
#include "gui/gui_section.h"
#include "utility/string_util.h"
#include "utility/file_util.h"
#include "scene/components/texture.h"


namespace titian
{
struct GUISectionExplorer : GUISection
{
    Texture default_file_texture;
    Texture mesh_file_texture;
    Texture texture_file_texture;
    Texture script_file_texture;
    Texture shader_file_texture;
    Texture scene_file_texture;
    Texture default_dir_texture;
    Texture parent_dir_texture;

    GUISectionExplorer();

    void render_gui() override;

private:
    String m_path = fs::absolute( "." ).string();
    float m_icon_size = 70.0f;

    void handle_file_entry( fs::path const& file );
    void handle_directory_entry( fs::path const& directory, bool is_parent_dir );

    void handle_item_transfer( StringRef const& item, StringRef const& destination );
    dx::ShaderView file_icon( FileType type );
};
}
