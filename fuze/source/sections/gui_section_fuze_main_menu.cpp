#include "fuze.h"


titian::GUISectionFuzeMainMenu::GUISectionFuzeMainMenu()
    : GUISection( "GUISectionFuzeMainMenu" )
{}

void titian::GUISectionFuzeMainMenu::render_gui()
{
    AppLayer& app_layer = AppLayer::get();
    VideoLayer& video_layer = VideoLayer::get();

    if ( m_testing_exit )
    {
        if ( im::Begin( "Exit?", nullptr, ImGuiWindowFlags_NoScrollbar ) )
        {
            im::Text( "Are you sure you want to exit?" );
            if ( im::Button( "No", ImVec2( 50.0f, 0.0f ) ) )
                m_testing_exit = false;

            im::SameLine();
            if ( im::Button( "Yes", ImVec2( 40.0f, 0.0f ) ) )
                app_layer.window.close();
        }
        im::End();
    }

    im::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
    im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0.0f, 8.0f ) );
    im::PushStyleColor( ImGuiCol_MenuBarBg, (ImVec4) ImColor( 0, 0, 0 ) );

    if ( im::BeginMainMenuBar() )
    {
        if ( im::BeginMenu( "File" ) )
        {
            im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2.0f, 2.0f ) );
            if ( im::MenuItem( "Import" ) )
            {
                if ( auto file = kl::choose_file( false ) )
                    video_layer.load_file( file.value() );
            }
            if ( im::BeginMenu( "Export" ) )
            {
                if ( im::MenuItem( "Frame" ) )
                {
                    int index = 0;
                    if ( auto file = kl::choose_file( true, { { "Image File", FILE_EXTENSION_JPG }, { "Image File", FILE_EXTENSION_PNG }, { "Image File", FILE_EXTENSION_BMP } }, &index ) )
                    {
                        video_layer.store_frame( video_layer.frame_size() );
                        RAWImage image{};
                        image.resize( video_layer.frame_size() );
                        video_layer.retrieve_frame( image );

                        RAWImageType image_type = RAWImageType::JPG;
                        if ( index == 1 )
                        {
                            image_type = RAWImageType::PNG;
                        }
                        else if ( index == 2 )
                        {
                            image_type = RAWImageType::BMP;
                        }
                        image.save_to_file( file.value(), image_type );
                    }
                }
                im::EndMenu();
            }

            im::Separator();
            if ( im::MenuItem( "Exit" ) )
                m_testing_exit = true;

            im::PopStyleVar();
            im::EndMenu();
        }

        if ( im::BeginMenu( "Edit" ) )
        {
            im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2.0f, 2.0f ) );
            //if ( im::MenuItem( "Undo" ) )
            //    ;
            //
            //if ( im::MenuItem( "Redo" ) )
            //    ;

            im::PopStyleVar();
            im::EndMenu();
        }

        if ( im::BeginMenu( "View" ) )
        {
            im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2.0f, 2.0f ) );
            im::Checkbox( "Timeline Seconds", &video_layer.timeline_seconds );
            im::Checkbox( "Timeline 10s Seconds", &video_layer.timeline_10seconds );
            im::Checkbox( "Timeline Minutes", &video_layer.timeline_minutes );
            im::PopStyleVar();
            im::EndMenu();
        }

        if ( im::BeginMenu( "About" ) )
        {
            im::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2.0f, 2.0f ) );
            im::Text( "Fuze 0.1v" );
            im::PopStyleVar();
            im::EndMenu();
        }

        im::EndMainMenuBar();
    }

    im::PopStyleColor();
    im::PopStyleVar( 2 );
}
