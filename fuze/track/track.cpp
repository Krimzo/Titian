#include "fuze.h"


titian::Track::Track()
{}

void titian::Track::insert_media( float offset, Ref<Media> const& new_med )
{
    if ( media.contains( offset ) )
        return;

    media[offset] = new_med;
    readjust_media();
}

void titian::Track::remove_media( Media const& new_med )
{
    for ( auto it = media.begin(); it != media.end(); it++ )
    {
        if ( &it->second == &new_med )
        {
            media.erase( it );
            break;
        }
    }
}

titian::Ref<titian::Media> titian::Track::get_media( float time, float& out_offset ) const
{
    for ( auto& [offset, med] : media )
    {
        if ( offset <= time && (offset + med->duration) >= time )
        {
            out_offset = offset;
            return med;
        }
    }
    out_offset = 0.0f;
    return {};
}

titian::Ref<titian::Track> titian::Track::make_copy() const
{
    Ref track = new Track();
    track->name = this->name;
    track->enabled = this->enabled;
    for ( auto& [offset, media] : this->media )
        track->media[offset] = media->make_copy();
    return track;
}

void titian::Track::readjust_media()
{
    std::map copy = media;
    media.clear();

    float last_offset = 0.0f;
    for ( auto& [offset, med] : copy )
    {
        float new_offset = (offset < last_offset) ? last_offset : offset;
        media[new_offset] = med;
        last_offset = new_offset + med->duration;
    }
}
