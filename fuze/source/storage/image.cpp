#include "fuze.h"


titian::Image::Image()
{}

titian::Image::Image( StringRef const& path )
{
    m_image.load_from_file( path );
}

void titian::Image::cache_frame( Int2 size )
{
    if ( out_frame.size() == size )
        return;

    RAWImage temp_frame = m_image;
    Int2 scaled_size = Float2( temp_frame.size() ) * kl::min( (float) size.x / temp_frame.width(), (float) size.y / temp_frame.height() );
    temp_frame.resize_scaled( scaled_size );

    out_frame.resize( size );
    out_frame.fill( RGB{ 0, 0, 0, 0 } );
    Int2 offset = kl::abs( (size - scaled_size) / 2 );

    kl::async_for( 0, temp_frame.pixel_count(), [&]( int i )
    {
        int x = i % temp_frame.width();
        int y = i / temp_frame.width();
        Int2 frame_coords = offset + Int2( x, y );
        if ( out_frame.in_bounds( frame_coords ) )
            out_frame[frame_coords] = temp_frame[{ x, y }];
    } );
}

titian::Ref<titian::Image> titian::Image::make_copy() const
{
    Ref image = new Image();
    image->m_image = this->m_image;
    return image;
}
