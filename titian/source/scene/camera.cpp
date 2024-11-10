#include "titian.h"


titian::Camera::Camera()
{
    resize( { 1600, 900 } );
    resize_staging( { 1, 1 } );
}

void titian::Camera::serialize( Serializer& serializer ) const
{
    Entity::serialize( serializer );

    serializer.write_int( "camera_type", camera_type );

    serializer.write_bool( "enabled", enabled );
    serializer.write_bool( "v_sync", v_sync );
    serializer.write_bool( "render_wireframe", render_wireframe );

    serializer.write_float( "aspect_ratio", aspect_ratio );
    serializer.write_float( "field_of_view", field_of_view );
    serializer.write_float( "width", width );
    serializer.write_float( "height", height );
    serializer.write_float( "near_plane", near_plane );
    serializer.write_float( "far_plane", far_plane );
    serializer.write_float( "sensitivity", sensitivity );
    serializer.write_float( "speed", speed );

    serializer.write_float_array( "forward", &m_forward.x, 3 );
    serializer.write_float_array( "up", &m_up.x, 3 );

    serializer.write_float_array( "background", &background.x, 4 );
    serializer.write_float_array( "custom_data", custom_data.data, 16 );

    serializer.write_string( "skybox_texture_name", skybox_texture_name );
    serializer.write_string( "target_texture_name", target_texture_name );
    serializer.write_string( "shader_name", shader_name );

    Int2 res = resolution();
    serializer.write_int_array( "resolution", &res.x, 2 );
}

void titian::Camera::deserialize( Serializer const& serializer )
{
    Entity::deserialize( serializer );

    serializer.read_int( "camera_type", (int32_t&) camera_type );

    serializer.read_bool( "enabled", enabled );
    serializer.read_bool( "v_sync", v_sync );
    serializer.read_bool( "render_wireframe", render_wireframe );

    serializer.read_float( "aspect_ratio", aspect_ratio );
    serializer.read_float( "field_of_view", field_of_view );
    serializer.read_float( "width", width );
    serializer.read_float( "height", height );
    serializer.read_float( "near_plane", near_plane );
    serializer.read_float( "far_plane", far_plane );
    serializer.read_float( "sensitivity", sensitivity );
    serializer.read_float( "speed", speed );

    serializer.read_float_array( "forward", &m_forward.x, 3 );
    serializer.read_float_array( "up", &m_up.x, 3 );

    serializer.read_float_array( "background", &background.x, 4 );
    serializer.read_float_array( "custom_data", custom_data.data, 16 );

    serializer.read_string( "skybox_texture_name", skybox_texture_name );
    serializer.read_string( "target_texture_name", target_texture_name );
    serializer.read_string( "shader_name", shader_name );

    Int2 res;
    serializer.read_int_array( "resolution", &res.x, 2 );
    resize( res );
}

void titian::Camera::update_aspect_ratio( Int2 size )
{
    aspect_ratio = size.x / float( size.y );
}

void titian::Camera::set_forward( Float3 const& dir )
{
    m_forward = normalize( dir );
}

void titian::Camera::set_up( Float3 const& dir )
{
    m_up = normalize( dir );
}

titian::Float3 titian::Camera::forward() const
{
    return m_forward;
}

titian::Float3 titian::Camera::right() const
{
    return cross( m_up, m_forward );
}

titian::Float3 titian::Camera::up() const
{
    return m_up;
}

void titian::Camera::move_forward( float delta_time )
{
    set_position( position() + m_forward * (speed * delta_time) );
}

void titian::Camera::move_back( float delta_time )
{
    set_position( position() - m_forward * (speed * delta_time) );
}

void titian::Camera::move_right( float delta_time )
{
    set_position( position() + right() * (speed * delta_time) );
}

void titian::Camera::move_left( float delta_time )
{
    set_position( position() - right() * (speed * delta_time) );
}

void titian::Camera::move_up( float delta_time )
{
    set_position( position() + m_up * (speed * delta_time) );
}

void titian::Camera::move_down( float delta_time )
{
    set_position( position() - m_up * (speed * delta_time) );
}

void titian::Camera::rotate( Float2 mouse_pos, Float2 frame_center, float vertical_angle_limit )
{
    Float2 rotation = (mouse_pos - frame_center) * sensitivity;
    Float3 forward_vert = kl::rotate( m_forward, right(), rotation.y );
    if ( kl::abs( angle( forward_vert, m_up ) - 90.0f ) <= vertical_angle_limit )
    {
        set_forward( forward_vert );
    }
    set_forward( kl::rotate( m_forward, m_up, rotation.x ) );
}

titian::Float4x4 titian::Camera::view_matrix() const
{
    Float3 position = this->position();
    return Float4x4::look_at( position, position + m_forward, m_up );
}

titian::Float4x4 titian::Camera::projection_matrix() const
{
    if ( camera_type == CameraType::ORTHOGRAPHIC )
    {
        return Float4x4::orthographic( -width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, near_plane, far_plane );
    }
    return Float4x4::perspective( field_of_view, aspect_ratio, near_plane, far_plane );
}

titian::Float4x4 titian::Camera::camera_matrix() const
{
    return projection_matrix() * view_matrix();
}

bool titian::Camera::can_see( Float3 const& point ) const
{
    return true;
}

void titian::Camera::resize( Int2 new_size )
{
    if ( new_size.x <= 0 || new_size.y <= 0 )
    {
        return;
    }
    if ( new_size == resolution() )
    {
        return;
    }
    kl::GPU const& gpu = AppLayer::get().gpu;

    dx::TextureDescriptor screen_desc{};
    screen_desc.Width = new_size.x;
    screen_desc.Height = new_size.y;
    screen_desc.MipLevels = 1;
    screen_desc.ArraySize = 1;
    screen_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    screen_desc.SampleDesc.Count = 1;
    screen_desc.Usage = D3D11_USAGE_DEFAULT;
    screen_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    screen_texture.texture = gpu.create_texture( &screen_desc, nullptr );
    screen_texture.create_target_view( nullptr );
    screen_texture.create_shader_view( nullptr );

    dx::TextureDescriptor color_desc = screen_desc;
    color_texture.texture = gpu.create_texture( &color_desc, nullptr );
    color_texture.create_target_view( nullptr );
    color_texture.create_shader_view( nullptr );

    dx::TextureDescriptor depth_desc = color_desc;
    depth_desc.Format = DXGI_FORMAT_R32_TYPELESS;
    depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    depth_texture.texture = gpu.create_texture( &depth_desc, nullptr );
    depth_staging.texture = gpu.create_staging_texture( depth_texture.texture );

    dx::DepthViewDescriptor depth_dv_desc{};
    depth_dv_desc.Format = DXGI_FORMAT_D32_FLOAT;
    depth_dv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_texture.create_depth_view( &depth_dv_desc );

    dx::ShaderViewDescriptor depth_sv_desc{};
    depth_sv_desc.Format = DXGI_FORMAT_R32_FLOAT;
    depth_sv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    depth_sv_desc.Texture2D.MipLevels = 1;
    depth_texture.create_shader_view( &depth_sv_desc );

    dx::TextureDescriptor index_desc = screen_desc;
    index_desc.Format = DXGI_FORMAT_R32_FLOAT;
    index_texture.texture = gpu.create_texture( &index_desc, nullptr );
    index_texture.create_target_view( nullptr );
    index_texture.create_shader_view( nullptr );
}

void titian::Camera::resize_staging( Int2 new_size )
{
    if ( new_size.x <= 0 || new_size.y <= 0 )
    {
        return;
    }
    if ( index_staging.resolution() == new_size )
    {
        return;
    }
    kl::GPU const& gpu = AppLayer::get().gpu;
    index_staging.texture = gpu.create_staging_texture( index_texture.texture, new_size );
}

titian::Int2 titian::Camera::resolution() const
{
    return screen_texture.resolution();
}

void titian::Camera::clear_targets()
{
    kl::GPU const& gpu = AppLayer::get().gpu;
    gpu.clear_target_view( color_texture.target_view, background );
    gpu.clear_depth_view( depth_texture.depth_view, 1.0f, 0xFF );
    gpu.clear_target_view( index_texture.target_view, {} );
}
