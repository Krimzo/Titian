#include "titian.h"


titian::GUISectionExplorer::GUISectionExplorer()
    : GUISection( "GUISectionExplorer" )
{
    auto create_texture = [&]( Texture& texture, str filename )
    {
        texture.image.load_from_file( filename );
        texture.reload_as_2D();
        texture.create_shader_view();
        kl::assert( texture.shader_view, "Failed to init texture: ", filename );
    };

    WorkQueue queue;
    queue.add_task( [&] { create_texture( default_file_texture, "package/textures/default_file.png" ); } );
    queue.add_task( [&] { create_texture( mesh_file_texture, "package/textures/mesh_file.png" ); } );
    queue.add_task( [&] { create_texture( texture_file_texture, "package/textures/texture_file.png" ); } );
    queue.add_task( [&] { create_texture( script_file_texture, "package/textures/script_file.png" ); } );
    queue.add_task( [&] { create_texture( shader_file_texture, "package/textures/shader_file.png" ); } );
    queue.add_task( [&] { create_texture( scene_file_texture, "package/textures/scene_file.png" ); } );
    queue.add_task( [&] { create_texture( default_dir_texture, "package/textures/default_dir.png" ); } );
    queue.add_task( [&] { create_texture( parent_dir_texture, "package/textures/parent_dir.png" ); } );
    queue.finalize();
}

void titian::GUISectionExplorer::render_gui()
{
    TimeBomb _ = bench_time_bomb();

    Vector<fs::path> directories;
    Vector<fs::path> files;
    try
    {
        for ( auto& entry : fs::directory_iterator( m_path ) )
            (entry.is_directory() ? directories : files).push_back( entry );
    }
    catch ( ... )
    {
    }

    if ( im::Begin( "Explorer", nullptr, ImGuiWindowFlags_NoScrollbar ) )
    {
        if ( im::BeginPopupContextWindow( "NewFile", ImGuiPopupFlags_MouseButtonMiddle ) )
        {
            im::Text( "Create" );
            String name_input = gui_input_continuous( "##CreateFileInput" );
            if ( !name_input.empty() )
            {
                if ( im::MenuItem( "Directory" ) )
                {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    String full_path = stream.str();
                    if ( !fs::exists( full_path ) )
                    {
                        fs::create_directory( full_path );
                        im::CloseCurrentPopup();
                    }
                }
                if ( im::MenuItem( "Text File" ) )
                {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    String full_path = stream.str();
                    if ( !fs::exists( full_path ) )
                    {
                        std::ofstream _{ full_path };
                        im::CloseCurrentPopup();
                    }
                }
                if ( im::MenuItem( "Script File" ) )
                {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    if ( name_input.find( FILE_EXTENSION_LUA ) == -1 )
                        stream << FILE_EXTENSION_LUA;

                    String full_path = stream.str();
                    if ( !fs::exists( full_path ) )
                    {
                        std::ofstream file{ full_path };
                        file << get_default_script();
                        im::CloseCurrentPopup();
                    }
                }
                if ( im::MenuItem( "Material Shader File" ) )
                {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    if ( name_input.find( FILE_EXTENSION_HLSL ) == -1 )
                        stream << FILE_EXTENSION_HLSL;

                    String full_path = stream.str();
                    if ( !fs::exists( full_path ) )
                    {
                        std::ofstream file{ full_path };
                        file << get_default_material_shader();
                        im::CloseCurrentPopup();
                    }
                }
                if ( im::MenuItem( "Camera Shader File" ) )
                {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    if ( name_input.find( FILE_EXTENSION_HLSL ) == -1 )
                        stream << FILE_EXTENSION_HLSL;

                    String full_path = stream.str();
                    if ( !fs::exists( full_path ) )
                    {
                        std::ofstream file{ full_path };
                        file << get_default_camera_shader();
                        im::CloseCurrentPopup();
                    }
                }
            }
            im::EndPopup();
        }

        GUILayer& gui_layer = GUILayer::get();
        float icon_size = m_icon_size * gui_layer.dpi_scaling;
        float window_width = im::GetWindowWidth() - im::GetStyle().WindowPadding.x * 2.0f;
        float icon_width = icon_size + im::GetStyle().CellPadding.x * 2.0f;
        int column_count = kl::max( (int) (window_width / icon_width), 1 );

        im::Text( m_path.data() );
        im::Separator();

        im::PushStyleVar( ImGuiStyleVar_CellPadding, ImVec2{ 0.0f, 4.0f } );

        if ( im::BeginTable( "##ExplorerTable", column_count ) )
        {
            fs::path current_path = { m_path };
            if ( current_path.has_parent_path() )
            {
                im::TableNextColumn();
                handle_directory_entry( current_path.parent_path(), true );
            }
            for ( auto& dir : directories )
            {
                im::TableNextColumn();
                handle_directory_entry( dir, false );
            }
            for ( auto& file : files )
            {
                im::TableNextColumn();
                handle_file_entry( file );
            }
            im::EndTable();
        }

        im::PopStyleVar();

        if ( im::IsKeyDown( ImGuiKey_LeftShift ) && im::BeginPopupContextWindow() )
        {
            im::SliderFloat( "Icon Size", &m_icon_size, 25.0f, 250.0f );
            im::EndPopup();
        }
    }
    im::End();
}

void titian::GUISectionExplorer::handle_file_entry( fs::path const& file )
{
    String path = fs::absolute( file ).string();
    FileType file_type = classify_file( file );
    dx::ShaderView icon = file_icon( file_type );

    constexpr float padding = 5.0f;
    im::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f } );
    im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ padding, padding } );

    float icon_size = m_icon_size * GUILayer::get().dpi_scaling;
    float text_height = im::CalcTextSize( path.data() ).y;

    ImVec2 child_size = { icon_size + padding * 2, icon_size + text_height + padding * 4.0f };
    if ( im::BeginChild( path.data(), child_size, ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar ) )
    {
        ImVec2 cursor_pos = im::GetCursorPos();
        if ( im::ImageButton( path.data(), icon.get(), { icon_size, icon_size } ) )
            ShellExecuteA( nullptr, nullptr, path.data(), nullptr, nullptr, 5 );

        gui_set_drag_drop<String>( DRAG_FILE_ID, path, icon );

        im::SetCursorPos( { cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f } );
        im::Text( file.filename().string().data() );
    }
    im::EndChild();

    im::PopStyleVar( 2 );

    if ( ImGui::IsItemHovered() )
        ImGui::SetMouseCursor( ImGuiMouseCursor_Hand );

    if ( im::BeginPopupContextItem( file.string().data(), ImGuiPopupFlags_MouseButtonRight ) )
    {
        if ( auto opt_name = gui_input_waited( "##RenameFileInput", file.filename().string() ) )
        {
            auto& name = opt_name.value();
            if ( !name.empty() )
            {
                fs::path new_file = file;
                new_file.replace_filename( opt_name.value() );

                if ( !fs::exists( new_file ) )
                {
                    fs::rename( file, new_file );
                    Logger::log( "Renamed file ", format_path( file ), " to ", format_path( new_file ) );
                    im::CloseCurrentPopup();
                }
                else
                {
                    Logger::log( "Failed to rename file ", format_path( file ), " to ", format_path( new_file ) );
                }
            }
        }
        if ( im::Button( "Delete", { -1.0f, 0.0f } ) )
        {
            Logger::log( "Deleted file ", format_path( file ) );
            fs::remove( file );
            im::CloseCurrentPopup();
        }
        else
        {
            Error ignored{};
            im::Text( format_byte_size( fs::file_size( file, ignored ) ).data() );
        }
        im::EndPopup();
    }
}

void titian::GUISectionExplorer::handle_directory_entry( fs::path const& dir, bool is_parent_dir )
{
    String path = fs::absolute( dir ).string();
    dx::ShaderView icon = is_parent_dir ? parent_dir_texture.shader_view : default_dir_texture.shader_view;

    constexpr float padding = 5.0f;
    im::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f } );
    im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ padding, padding } );

    float icon_size = m_icon_size * GUILayer::get().dpi_scaling;
    float text_height = im::CalcTextSize( path.data() ).y;

    ImVec2 child_size = { icon_size + padding * 2, icon_size + text_height + padding * 4.0f };
    if ( im::BeginChild( path.data(), child_size, ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar ) )
    {
        ImVec2 cursor_pos = im::GetCursorPos();
        if ( im::ImageButton( path.data(), icon.get(), { icon_size, icon_size } ) )
            m_path = path;

        if ( auto dragged_path = gui_get_drag_drop<String>( DRAG_FILE_ID ) )
            handle_item_transfer( dragged_path.value(), path );

        if ( auto dragged_path = gui_get_drag_drop<String>( DRAG_DIR_ID ) )
            handle_item_transfer( dragged_path.value(), path );

        if ( !is_parent_dir )
            gui_set_drag_drop<String>( DRAG_DIR_ID, path, icon );

        im::SetCursorPos( { cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f } );
        im::Text( dir.filename().string().data() );
    }
    im::EndChild();

    im::PopStyleVar( 2 );

    if ( ImGui::IsItemHovered() )
        ImGui::SetMouseCursor( ImGuiMouseCursor_Hand );

    if ( !is_parent_dir && im::BeginPopupContextItem( dir.string().data(), ImGuiPopupFlags_MouseButtonRight ) )
    {
        if ( auto opt_name = gui_input_waited( "##RenameDirInput", dir.filename().string() ) )
        {
            auto& name = opt_name.value();
            if ( !name.empty() )
            {
                fs::path new_dir = dir;
                new_dir.replace_filename( opt_name.value() );

                if ( !fs::exists( new_dir ) )
                {
                    fs::rename( dir, new_dir );
                    Logger::log( "Renamed directory ", format_path( dir ), " to ", format_path( new_dir ) );
                    im::CloseCurrentPopup();
                }
                else
                {
                    Logger::log( "Failed to rename directory ", format_path( dir ), " to ", format_path( new_dir ) );
                }
            }
        }
        if ( im::Button( "Delete", { -1.0f, 0.0f } ) )
        {
            Logger::log( "Deleted directory ", format_path( dir ) );
            fs::remove_all( dir );
            im::CloseCurrentPopup();
        }
        im::EndPopup();
    }
}

void titian::GUISectionExplorer::handle_item_transfer( StringRef const& item, StringRef const& destination )
{
    auto item_absolute = fs::absolute( item );
    auto destination_absolute = fs::absolute( destination );
    if ( item_absolute == destination_absolute )
    {
        Logger::log( "Failed to transfer file ", format_path( item_absolute ), " to ", format_path( destination_absolute ) );
        return;
    }

    String new_item_absolute = destination_absolute.string() + "/" + item_absolute.filename().string();
    if ( fs::exists( new_item_absolute ) )
    {
        Logger::log( "Failed to transfer file ", format_path( item_absolute ), " to ", format_path( new_item_absolute ) );
        return;
    }

    fs::rename( item_absolute, new_item_absolute );
    Logger::log( "Transferred file ", format_path( item_absolute ), " to ", format_path( new_item_absolute ) );
}

dx::ShaderView titian::GUISectionExplorer::file_icon( FileType type )
{
    switch ( type )
    {
    case FileType::MESH:
        return mesh_file_texture.shader_view;

    case FileType::TEXTURE:
        return texture_file_texture.shader_view;

    case FileType::SCRIPT:
        return script_file_texture.shader_view;

    case FileType::SHADER:
        return shader_file_texture.shader_view;

    case FileType::BINARY_SCENE:
    case FileType::TEXT_SCENE:
        return scene_file_texture.shader_view;
    }
    return default_file_texture.shader_view;
}
