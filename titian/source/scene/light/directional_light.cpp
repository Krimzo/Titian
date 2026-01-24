#include "titian.h"


titian::DirectionalLight::DirectionalLight()
{
    kl::copy<float>( cascade_ends, Defaults::CASCADE_ENDS, std::size( Defaults::CASCADE_ENDS ) );
    set_resolution( Defaults::RESOLUTION );
}

void titian::DirectionalLight::serialize( Serializer& serializer ) const
{
    Entity::serialize( serializer );

    serializer.write_float_array( "color", &color.x, 3 );
    serializer.write_float( "point_size", point_size );
    serializer.write_float_array( "cascade_ends", cascade_ends, (int) std::size( cascade_ends ) );

    serializer.write_int( "resolution", m_resolution );
    serializer.write_float_array( "direction", &m_direction.x, 3 );
}

void titian::DirectionalLight::deserialize( Serializer const& serializer )
{
    Entity::deserialize( serializer );

    serializer.read_float_array( "color", &color.x, 3 );
    serializer.read_float( "point_size", point_size );
    serializer.read_float_array( "cascade_ends", cascade_ends, (int) std::size( cascade_ends ) );

    serializer.read_int( "resolution", m_resolution );
    serializer.read_float_array( "direction", &m_direction.x, 3 );

    set_resolution( m_resolution );
}

void titian::DirectionalLight::set_resolution( int resolution )
{
    m_resolution = resolution;

    dx::TextureDescriptor shadow_map_descriptor{};
    shadow_map_descriptor.Width = resolution;
    shadow_map_descriptor.Height = resolution;
    shadow_map_descriptor.MipLevels = 1;
    shadow_map_descriptor.ArraySize = 1;
    shadow_map_descriptor.Format = DXGI_FORMAT_R32_TYPELESS;
    shadow_map_descriptor.SampleDesc.Count = 1;
    shadow_map_descriptor.Usage = D3D11_USAGE_DEFAULT;
    shadow_map_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    dx::DepthViewDescriptor shadow_depth_view_descriptor{};
    shadow_depth_view_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
    shadow_depth_view_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    dx::ShaderViewDescriptor shadow_shader_view_descriptor{};
    shadow_shader_view_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
    shadow_shader_view_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shadow_shader_view_descriptor.Texture2D.MipLevels = 1;

    kl::GPU& gpu = AppLayer::get().gpu;
    for ( auto& cascade : m_cascades )
    {
        cascade.texture = gpu.create_texture( &shadow_map_descriptor, nullptr );
        cascade.create_depth_view( &shadow_depth_view_descriptor );
        cascade.create_shader_view( &shadow_shader_view_descriptor );
    }
}

int titian::DirectionalLight::resolution() const
{
    return m_resolution;
}

void titian::DirectionalLight::set_direction( Float3 const& direction )
{
    m_direction = kl::normalize( direction );
}

titian::Float3 titian::DirectionalLight::direction() const
{
    return m_direction;
}

dx::DepthView titian::DirectionalLight::depth_view( int cascade_index ) const
{
    return m_cascades[cascade_index].depth_view;
}

dx::ShaderView titian::DirectionalLight::shader_view( int cascade_index ) const
{
    return m_cascades[cascade_index].shader_view;
}

titian::Float4x4 titian::DirectionalLight::light_matrix( Float4x4 const& inv_cam_mat ) const
{
    Float4 frustum_corners[8] = {
        { -1, -1, 0, 1 },
        { 1, -1, 0, 1 },
        { -1, 1, 0, 1 },
        { 1, 1, 0, 1 },
        { -1, -1, 1, 1 },
        { 1, -1, 1, 1 },
        { -1, 1, 1, 1 },
        { 1, 1, 1, 1 },
    };
    for ( auto& corner : frustum_corners )
    {
        corner = inv_cam_mat * corner;
        corner *= 1.0f / corner.w;
    }

    const Float4x4 temp_light_view_matrix = Float4x4::look_to( {}, m_direction, { 0, 1, 0 } );
    for ( auto& corner : frustum_corners )
        corner = temp_light_view_matrix * corner;

    Float2 min_xy{ std::numeric_limits<float>::infinity() };
    Float2 max_xy{ -std::numeric_limits<float>::infinity() };
    float min_z = std::numeric_limits<float>::infinity();
    for ( auto& corner : frustum_corners )
    {
        min_xy.x = kl::min( min_xy.x, corner.x );
        min_xy.y = kl::min( min_xy.y, corner.y );
        max_xy.x = kl::max( max_xy.x, corner.x );
        max_xy.y = kl::max( max_xy.y, corner.y );
        min_z = kl::min( min_z, corner.z );
    }
    Float3 light_position = {
        ( min_xy.x + max_xy.x ) * 0.5f,
        ( min_xy.y + max_xy.y ) * 0.5f,
        min_z
    };

    const Float4x4 temp_light_view_matrix_inverse = inverse( temp_light_view_matrix );
    light_position = ( temp_light_view_matrix_inverse * Float4( light_position, 1.0f ) ).xyz();
    for ( auto& corner : frustum_corners )
        corner = temp_light_view_matrix_inverse * corner;

    const Float4x4 light_view_matrix = Float4x4::look_to( light_position, m_direction, { 0, 1, 0 } );
    for ( auto& corner : frustum_corners )
        corner = light_view_matrix * corner;

    Float3 max_xyz{ -std::numeric_limits<float>::infinity() };
    min_z = std::numeric_limits<float>::infinity();
    for ( auto& corner : frustum_corners )
    {
        max_xyz.x = kl::max( max_xyz.x, kl::abs( corner.x ) );
        max_xyz.y = kl::max( max_xyz.y, kl::abs( corner.y ) );
        max_xyz.z = kl::max( max_xyz.z, corner.z );
        min_z = kl::min( min_z, corner.z );
    }

    const Float4x4 light_projection_matrix = Float4x4::orthographic(
        max_xyz.x * 2.0f,
        max_xyz.y * 2.0f,
        min_z, max_xyz.z
    );
    return light_projection_matrix * light_view_matrix;
}

titian::Float4x4 titian::DirectionalLight::light_matrix_cascade( Camera& camera, int cascade_index ) const
{
    const Float2 old_camera_planes = { camera.near_plane, camera.far_plane };
    camera.near_plane = kl::lerp( cascade_index > 0 ? cascade_ends[cascade_index - 1] : 0.0f, old_camera_planes.x, old_camera_planes.y );
    camera.far_plane = kl::lerp( cascade_ends[cascade_index], old_camera_planes.x, old_camera_planes.y );
    const Float4x4 inv_cam_mat = kl::inverse( camera.camera_matrix() );
    camera.near_plane = old_camera_planes.x;
    camera.far_plane = old_camera_planes.y;
    return light_matrix( inv_cam_mat );
}
