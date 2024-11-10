#include "titian.h"


titian::GUISectionSceneEntities::GUISectionSceneEntities()
    : GUISection( "GUISectionSceneEntities" )
{}

void titian::GUISectionSceneEntities::render_gui()
{
    TimeBomb _ = bench_time_bomb();

    EditorLayer& editor_layer = EditorLayer::get();
    kl::GPU& gpu = AppLayer::get().gpu;
    Scene& scene = GameLayer::get().scene();

    if ( im::Begin( "Scene Entities" ) )
    {
        if ( im::BeginPopupContextWindow( "NewEntity", ImGuiPopupFlags_MouseButtonMiddle ) )
        {
            String name = gui_input_continuous( "##CreateEntityInput" );
            if ( !name.empty() && !scene.helper_contains_entity( name ) )
            {
                if ( im::BeginMenu( "Basic" ) )
                {
                    if ( im::MenuItem( "New Entity" ) )
                    {
                        Ref entity = new Entity();
                        scene.add_entity( name, entity );
                        im::CloseCurrentPopup();
                    }
                    if ( im::MenuItem( "New Camera" ) )
                    {
                        Ref entity = new Camera();
                        scene.add_entity( name, entity );
                        im::CloseCurrentPopup();
                    }
                    if ( im::MenuItem( "New Ambient Light" ) )
                    {
                        Ref entity = new AmbientLight();
                        scene.add_entity( name, entity );
                        im::CloseCurrentPopup();
                    }
                    if ( im::MenuItem( "New Directional Light" ) )
                    {
                        Ref entity = new DirectionalLight();
                        scene.add_entity( name, entity );
                        im::CloseCurrentPopup();
                    }
                    im::EndMenu();
                }
                if ( im::BeginMenu( "Animation" ) )
                {
                    for ( auto& [anim_name, animation] : scene.animations )
                    {
                        if ( im::MenuItem( kl::format( anim_name, "##AnimationEnt" ).data() ) )
                        {
                            Ref entity = new Entity();
                            entity->animation_name = anim_name;
                            entity->material_name = "white";
                            scene.add_entity( name, entity );
                            im::CloseCurrentPopup();
                        }
                    }
                    im::EndMenu();
                }
            }
            im::EndPopup();
        }

        if ( im::IsWindowFocused() && im::IsMouseHoveringRect( im::GetWindowPos(), im::GetWindowPos() + im::GetWindowSize() ) )
        {
            if ( im::IsKeyPressed( ImGuiKey_Delete ) )
            {
                for ( auto& name : editor_layer.selected_entities )
                    scene.remove_entity( name );
                editor_layer.selected_entities.clear();
            }
        }

        String filter = gui_input_continuous( "Search###SceneEntities" );
        for ( auto& [entity_name, entity] : scene.entities() )
        {
            if ( !filter.empty() && !str_find( entity_name, filter ) )
                continue;

            if ( im::Selectable( entity_name.data(), editor_layer.selected_entities.contains( entity_name ) ) )
            {
                if ( im::IsKeyDown( ImGuiKey_LeftCtrl ) )
                {
                    if ( editor_layer.selected_entities.contains( entity_name ) )
                    {
                        editor_layer.selected_entities.erase( entity_name );
                    }
                    else
                    {
                        editor_layer.selected_entities.insert( entity_name );
                    }
                }
                else if ( im::IsKeyDown( ImGuiKey_LeftShift ) )
                {
                    if ( editor_layer.selected_entities.empty() )
                    {
                        editor_layer.selected_entities.insert( entity_name );
                    }
                    else
                    {
                        for ( int find_counter = 0; auto & [name, _] : scene.entities() )
                        {
                            if ( !filter.empty() && !str_find( name, filter ) )
                                continue;

                            if ( name == entity_name || name == *--editor_layer.selected_entities.end() )
                                find_counter += 1;

                            if ( find_counter > 0 )
                                editor_layer.selected_entities.insert( name );

                            if ( find_counter >= 2 )
                                break;
                        }
                    }
                }
                else
                {
                    editor_layer.selected_entities = { entity_name };
                }
            }

            if ( im::BeginPopupContextItem( entity_name.data(), ImGuiPopupFlags_MouseButtonRight ) )
            {
                bool should_break = false;
                im::Text( "Edit Entity" );

                if ( auto opt_name = gui_input_waited( "##RenameEntityInput", entity_name ) )
                {
                    auto& new_name = opt_name.value();
                    if ( !new_name.empty() && !scene.helper_contains_entity( new_name ) )
                    {
                        if ( scene.main_camera_name == entity_name )
                            scene.main_camera_name = new_name;

                        if ( scene.main_ambient_light_name == entity_name )
                            scene.main_ambient_light_name = new_name;

                        if ( scene.main_directional_light_name == entity_name )
                            scene.main_directional_light_name = new_name;

                        Ref temp_ent = entity;
                        scene.remove_entity( entity_name );
                        scene.add_entity( new_name, temp_ent );
                        should_break = true;
                        im::CloseCurrentPopup();
                    }
                }

                if ( im::Button( "Delete", { -1.0f, 0.0f } ) )
                {
                    scene.remove_entity( entity_name );
                    should_break = true;
                    im::CloseCurrentPopup();
                }

                im::EndPopup();
                if ( should_break )
                    break;
            }
        }
    }
    im::End();
}
