#include "klibrary.h"


kl::Texture::Texture( GPU const& gpu )
    : m_gpu( gpu )
{}

void kl::Texture::reload( bool has_unordered_access, bool is_target )
{
    texture = m_gpu.create_texture( image, has_unordered_access, is_target );
}

bool kl::Texture::reload_as_cube()
{
    if ( image.width() % 4 || image.height() % 3 )
        return false;

    int part_size = image.width() / 4;
    Image box_sides[6] = {
        image.rectangle( Int2( 2, 1 ) * part_size, Int2( 3, 2 ) * part_size ),
        image.rectangle( Int2( 0, 1 ) * part_size, Int2( 1, 2 ) * part_size ),
        image.rectangle( Int2( 1, 0 ) * part_size, Int2( 2, 1 ) * part_size ),
        image.rectangle( Int2( 1, 2 ) * part_size, Int2( 2, 3 ) * part_size ),
        image.rectangle( Int2( 1, 1 ) * part_size, Int2( 2, 2 ) * part_size ),
        image.rectangle( Int2( 3, 1 ) * part_size, Int2( 4, 2 ) * part_size ),
    };
    texture = m_gpu.create_cube_texture( box_sides[0], box_sides[1], box_sides[2], box_sides[3], box_sides[4], box_sides[5] );
    return true;
}

void kl::Texture::create_target_view( dx::TargetViewDescriptor const* descriptor )
{
    target_view = m_gpu.create_target_view( texture, descriptor );
}

void kl::Texture::create_depth_view( dx::DepthViewDescriptor const* descriptor )
{
    depth_view = m_gpu.create_depth_view( texture, descriptor );
}

void kl::Texture::create_shader_view( dx::ShaderViewDescriptor const* descriptor )
{
    shader_view = m_gpu.create_shader_view( texture, descriptor );
}

void kl::Texture::create_access_view( dx::AccessViewDescriptor const* descriptor )
{
    access_view = m_gpu.create_access_view( texture, descriptor );
}

bool kl::Texture::is_cube() const
{
    if ( !texture )
        return false;

    dx::TextureDescriptor desc{};
    texture->GetDesc( &desc );
    return desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE;
}

kl::Int2 kl::Texture::resolution() const
{
    if ( !texture )
        return {};

    dx::TextureDescriptor desc{};
    texture->GetDesc( &desc );
    return { int( desc.Width ), int( desc.Height ) };
}
