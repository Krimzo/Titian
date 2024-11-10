#include "titian.h"


titian::GUISectionHelper::GUISectionHelper()
    : GUISection( "GUISectionHelper" )
{
    auto create_texture = [&]( Texture& texture, str filename )
    {
        texture.image.load_from_file( filename );
        texture.reload_as_2D();
        texture.create_shader_view();
        kl::assert( texture.shader_view, "Failed to init texture: ", filename );
    };

    WorkQueue queue;
    queue.add_task( [&] { create_texture( basic_entity_texture, "package/textures/scene_file.png" ); } );
    queue.add_task( [&] { create_texture( animation_entity_texture, "package/textures/mesh_file.png" ); } );
    queue.finalize();
}

void titian::GUISectionHelper::render_gui()
{
    TimeBomb _ = bench_time_bomb();

    GUILayer const& gui_layer = GUILayer::get();
    Scene const& scene = GameLayer::get().scene();

    if ( im::Begin( "Helper", nullptr, ImGuiWindowFlags_NoScrollbar ) )
    {
        float icon_size = m_icon_size * gui_layer.dpi_scaling;
        float window_width = im::GetWindowWidth() - im::GetStyle().WindowPadding.x * 2.0f;
        float icon_width = icon_size + im::GetStyle().CellPadding.x * 2.0f;
        int column_count = kl::max( (int) (window_width / icon_width), 1 );

        im::PushStyleVar( ImGuiStyleVar_CellPadding, ImVec2{ 0.0f, 4.0f } );

        if ( im::BeginTable( "##HelperTable", column_count ) )
        {
            for ( auto& type : { DRAG_ENTITY_ENTITY, DRAG_ENTITY_CAMERA, DRAG_ENTITY_AMBIENT, DRAG_ENTITY_DIRECTIONAL } )
            {
                im::TableNextColumn();
                handle_basic_entry( type );
            }
            for ( auto& [name, _] : scene.animations )
            {
                im::TableNextColumn();
                handle_animation_entry( name );
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

void titian::GUISectionHelper::handle_basic_entry( StringRef const& name ) const
{
    constexpr float padding = 5.0f;
    im::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f } );
    im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ padding, padding } );

    dx::ShaderView icon = basic_entity_texture.shader_view;
    float icon_size = m_icon_size * GUILayer::get().dpi_scaling;
    float text_height = im::CalcTextSize( name.data() ).y;

    if ( im::BeginChild( name.data(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar ) )
    {
        ImVec2 cursor_pos = im::GetCursorPos();
        im::ImageButton( name.data(), icon.get(), { icon_size, icon_size } );
        gui_set_drag_drop<String>( DRAG_BASIC_ENTITY_ID, String( name ), icon );
        im::SetCursorPos( { cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f } );
        im::Text( name.data() );
    }
    im::EndChild();

    im::PopStyleVar( 2 );

    if ( ImGui::IsItemHovered() )
        ImGui::SetMouseCursor( ImGuiMouseCursor_Hand );
}

void titian::GUISectionHelper::handle_animation_entry( StringRef const& name ) const
{
    constexpr float padding = 5.0f;
    im::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f } );
    im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ padding, padding } );

    dx::ShaderView icon = animation_entity_texture.shader_view;
    float icon_size = m_icon_size * GUILayer::get().dpi_scaling;
    float text_height = im::CalcTextSize( name.data() ).y;

    if ( im::BeginChild( name.data(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar ) )
    {
        ImVec2 cursor_pos = im::GetCursorPos();
        im::ImageButton( name.data(), icon.get(), { icon_size, icon_size } );
        gui_set_drag_drop<String>( DRAG_ANIMATION_ENTITY_ID, String( name ), icon );
        im::SetCursorPos( { cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f } );
        im::Text( name.data() );
    }
    im::EndChild();

    im::PopStyleVar( 2 );

    if ( ImGui::IsItemHovered() )
        ImGui::SetMouseCursor( ImGuiMouseCursor_Hand );
}
