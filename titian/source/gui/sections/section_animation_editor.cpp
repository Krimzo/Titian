#include "titian.h"


titian::GUISectionAnimationEditor::GUISectionAnimationEditor()
    : GUISection( "GUISectionAnimationEditor" )
{
    sun_direction = kl::normalize( Float3( -0.5f, -0.75f, 1.0f ) );
    camera.background = RGB{ 30, 30, 30 };
    camera.set_position( { -0.34f, 0.18f, -0.94f } );
    camera.speed = 3.1f;
    m_timer.reset();
}

void titian::GUISectionAnimationEditor::render_gui()
{
    TimeBomb _ = bench_time_bomb();

    kl::GPU& gpu = AppLayer::get().gpu;
    Scene& scene = GameLayer::get().scene();

    m_timer.update();

    Ref<Animation> animation;
    if ( scene.animations.contains( selected_animation ) )
        animation = scene.animations.at( selected_animation );

    if ( im::Begin( "Animation Editor" ) )
    {
        float available_width = im::GetContentRegionAvail().x;
        im::Columns( 2, "AnimationEditorColumns", false );

        im::SetColumnWidth( im::GetColumnIndex(), available_width * 0.25f );
        if ( im::BeginChild( "Animations" ) )
            display_animations( scene );

        im::EndChild();
        im::NextColumn();

        im::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 1.0f );
        im::PushStyleVar( ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f } );
        im::PushStyleColor( ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f } );

        if ( im::BeginChild( "Animation View", {}, was_focused ) )
        {
            Int2 viewport_size = { (int) im::GetContentRegionAvail().x, (int) im::GetContentRegionAvail().y };
            if ( was_focused )
                update_animation_camera( scene );

            if ( animation )
            {
                render_selected_animation( *animation, viewport_size );
                dx::ShaderView const& shader_view = render_texture.shader_view;
                im::Image( render_texture.shader_view.get(), { (float) viewport_size.x, (float) viewport_size.y } );
            }
            was_focused = im::IsWindowFocused();
        }
        im::EndChild();

        im::PopStyleColor();
        im::PopStyleVar( 2 );

        show_animation_properties( &animation );
    }
    im::End();
}

void titian::GUISectionAnimationEditor::display_animations( Scene& scene )
{
    if ( im::BeginPopupContextWindow( "NewAnimation", ImGuiPopupFlags_MouseButtonMiddle ) )
    {
        im::Text( "New Animation" );
        String name = gui_input_continuous( "##CreateAnimationInput" );
        if ( !name.empty() && !scene.animations.contains( name ) )
        {
            if ( im::MenuItem( "Basic Animation" ) )
            {
                scene.animations[name] = new Animation();
                im::CloseCurrentPopup();
            }
            if ( im::BeginMenu( "Mesh Animation" ) )
            {
                String filter = gui_input_continuous( "Search###NewMeshAnimation" );
                for ( auto& [mesh_name, _] : scene.meshes )
                {
                    if ( !filter.empty() && !str_find( mesh_name, filter ) )
                        continue;

                    if ( im::Selectable( mesh_name.data(), false ) )
                    {
                        Ref animation = new Animation();
                        animation->meshes = { mesh_name };
                        scene.animations[name] = animation;
                        im::CloseCurrentPopup();
                    }
                }
                im::EndMenu();
            }
        }
        im::EndPopup();
    }

    String filter = gui_input_continuous( "Search###AnimationEditor" );
    for ( auto& [animation_name, animation] : scene.animations )
    {
        if ( !filter.empty() && !str_find( animation_name, filter ) )
            continue;

        if ( im::Selectable( animation_name.data(), animation_name == selected_animation ) )
            selected_animation = animation_name;

        if ( im::BeginPopupContextItem( animation_name.data(), ImGuiPopupFlags_MouseButtonRight ) )
        {
            bool should_break = false;
            im::Text( "Edit Animation" );

            if ( auto opt_name = gui_input_waited( "##RenameAnimationInput", animation_name ) )
            {
                auto& new_name = opt_name.value();
                if ( !new_name.empty() && !scene.animations.contains( new_name ) )
                {
                    if ( selected_animation == animation_name )
                        selected_animation = new_name;

                    for ( auto& [_, entity] : scene.entities() )
                    {
                        if ( entity->animation_name == animation_name )
                            entity->animation_name = new_name;
                    }
                    scene.animations[new_name] = animation;
                    scene.animations.erase( animation_name );
                    should_break = true;
                    im::CloseCurrentPopup();
                }
            }

            if ( im::Button( "Delete", { -1.0f, 0.0f } ) )
            {
                if ( selected_animation == animation_name )
                    selected_animation = "/";

                scene.animations.erase( animation_name );
                should_break = true;
                im::CloseCurrentPopup();
            }

            im::EndPopup();
            if ( should_break )
                break;
        }
    }
}

void titian::GUISectionAnimationEditor::update_animation_camera( Scene& scene )
{
    if ( im::IsMouseClicked( ImGuiMouseButton_Right ) )
        initial_camera_info = camera_info;

    int scroll = AppLayer::get().window.mouse.scroll();
    if ( im::IsMouseDown( ImGuiMouseButton_Right ) )
    {
        ImVec2 drag_delta = im::GetMouseDragDelta( ImGuiMouseButton_Right );
        camera_info.x = initial_camera_info.x + drag_delta.x * camera.sensitivity;
        camera_info.y = initial_camera_info.y + drag_delta.y * camera.sensitivity;
        camera_info.y = kl::clamp( camera_info.y, -85.0f, 85.0f );

        camera.set_position( {
            kl::sin_d( camera_info.x ),
            kl::tan_d( camera_info.y ),
            kl::cos_d( camera_info.x ),
            } );

        camera.speed -= scroll * 0.1f;
        camera.speed = kl::max( camera.speed, 0.1f );
    }

    float camera_distance = camera.speed;
    camera.set_position( kl::normalize( camera.position() ) * camera_distance );
    camera.set_forward( -camera.position() );
}

void titian::GUISectionAnimationEditor::render_selected_animation( Animation& animation, Int2 viewport_size )
{
    if ( viewport_size.x <= 0 || viewport_size.y <= 0 )
        return;

    RenderLayer& render_layer = RenderLayer::get();
    kl::GPU& gpu = AppLayer::get().gpu;
    Scene& scene = GameLayer::get().scene();

    if ( render_texture.resolution() != viewport_size )
    {
        render_texture.texture = gpu.create_target_texture( viewport_size );
        render_texture.create_target_view( nullptr );
        render_texture.create_shader_view( nullptr );
    }

    if ( depth_texture.resolution() != viewport_size )
    {
        dx::TextureDescriptor descriptor = {};
        descriptor.Width = viewport_size.x;
        descriptor.Height = viewport_size.y;
        descriptor.MipLevels = 1;
        descriptor.ArraySize = 1;
        descriptor.Format = DXGI_FORMAT_D32_FLOAT;
        descriptor.SampleDesc.Count = 1;
        descriptor.Usage = D3D11_USAGE_DEFAULT;
        descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depth_texture.texture = gpu.create_texture( &descriptor, nullptr );
        depth_texture.create_depth_view( nullptr );
    }

    gpu.clear_target_view( render_texture.target_view, camera.background );
    gpu.clear_depth_view( depth_texture.depth_view, 1.0f, 0xFF );

    Mesh* mesh = nullptr;
    if ( animation.animation_type == AnimationType::SEQUENTIAL )
    {
        if ( m_animating )
            m_frame_index = animation.get_index( m_timer.elapsed() );

        if ( m_frame_index >= 0 && m_frame_index < (int) animation.meshes.size() )
            mesh = scene.helper_get_mesh( animation.meshes[m_frame_index] );
    }
    else
    {
        mesh = animation.get_mesh( scene, 0.0f );
    }
    if ( !mesh )
        return;

    gpu.bind_target_depth_view( render_texture.target_view, depth_texture.depth_view );

    Int2 old_viewport_size = gpu.viewport_size();
    gpu.set_viewport_size( viewport_size );

    gpu.bind_raster_state( mesh->render_wireframe ? render_layer.raster_states.wireframe : render_layer.raster_states.solid );
    gpu.bind_depth_state( render_layer.depth_states.enabled );

    kl::Shaders& shaders = render_layer.shader_states.solid_lit_pass;
    gpu.bind_shaders( shaders );

    camera.update_aspect_ratio( viewport_size );

    struct alignas(16) CB
    {
        Float4x4 W;
        Float4x4 WVP;
        Float4 OBJECT_COLOR;
        Float3 SUN_DIRECTION;
        float IS_SKELETAL;
    } cb = {};

    cb.WVP = camera.camera_matrix();
    cb.OBJECT_COLOR = line_color;
    cb.SUN_DIRECTION = sun_direction;

    if ( m_animating && animation.animation_type == AnimationType::SKELETAL )
    {
        animation.update( scene, m_timer.elapsed() );
        animation.bind_matrices( 0 );
        cb.IS_SKELETAL = 1.0f;
    }

    shaders.upload( cb );
    gpu.draw( mesh->buffer, mesh->d3d_topology(), sizeof( Vertex ) );

    gpu.unbind_shader_view_for_vertex_shader( 0 );
    gpu.bind_internal_views();
    gpu.set_viewport_size( old_viewport_size );
}

void titian::GUISectionAnimationEditor::show_animation_properties( Animation* animation )
{
    GUILayer& gui_layer = GUILayer::get();
    kl::Window& window = AppLayer::get().window;
    Scene& scene = GameLayer::get().scene();

    if ( im::Begin( "Animation Properties" ) && animation )
    {
        if ( im::IsWindowFocused() )
        {
            m_start_mesh_index += window.mouse.scroll();
            m_start_mesh_index = kl::clamp( m_start_mesh_index, 0, kl::max<int>( (int) animation->meshes.size() - 1, 0 ) );
        }

        im::Text( "Animation Editor" );
        if ( im::DragFloat3( "Sun Direction", &sun_direction.x, 0.01f ) )
            sun_direction = kl::normalize( sun_direction );

        im::SliderInt( "Frame", &m_frame_index, 0, kl::max<int>( (int) animation->meshes.size() - 1, 0 ) );

        float duration_seconds = (animation->animation_type == AnimationType::SKELETAL)
            ? (animation->duration_in_ticks / animation->ticks_per_second)
            : (animation->meshes.size() / animation->ticks_per_second);
        float temp_anim_time = fmod( m_timer.elapsed(), duration_seconds );
        im::SliderFloat( "Time", &temp_anim_time, 0.0f, duration_seconds );

        if ( im::Checkbox( "Animating", &m_animating ) )
        {
            if ( m_animating )
            {
                m_timer.restart();
            }
            else
            {
                m_timer.reset();
            }
        }

        im::Separator();

        im::Text( "Info" );
        im::Text( "Name: " );
        im::SameLine();
        gui_colored_text( selected_animation, gui_layer.special_color );

        static Map<int32_t, String> animation_type_names{
            { AnimationType::SEQUENTIAL, "Sequential" },
            { AnimationType::SKELETAL, "Skeletal" },
        };

        if ( im::BeginCombo( "Animation Type", animation_type_names.at( animation->animation_type ).data() ) )
        {
            for ( auto& [type, name] : animation_type_names )
            {
                if ( im::Selectable( name.data(), animation->animation_type == type ) )
                {
                    animation->animation_type = type;
                    m_start_mesh_index = 0;
                }
            }
            im::EndCombo();
        }

        im::DragFloat( "Ticks Per Second", &animation->ticks_per_second, 1.0f, 0.0f, 10'000.0f );
        if ( animation->animation_type == AnimationType::SKELETAL )
        {
            im::DragFloat( "Duration in Ticks", &animation->duration_in_ticks, 1.0f, 1.0f, 10'000.0f );
            animation->meshes.resize( 1 );
        }
        else
        {
            int mesh_count = (int) animation->meshes.size();
            if ( im::DragInt( "Mesh Count", &mesh_count, 1.0f, 0, 10'000 ) )
                animation->meshes.resize( mesh_count );
        }

        for ( int i = m_start_mesh_index; i < (m_start_mesh_index + 10) && i < (int) animation->meshes.size(); i++ )
        {
            if ( im::BeginCombo( kl::format( i, ". Mesh" ).data(), animation->meshes[i].data() ) )
            {
                for ( auto& [name, _] : scene.meshes )
                {
                    if ( im::Selectable( name.data(), name == animation->meshes[i] ) )
                        animation->meshes[i] = name;
                }
                im::EndCombo();
            }
        }

    }
    im::End();
}
