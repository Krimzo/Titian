#include "fuze.h"


titian::Audio::Audio()
{}

titian::Audio::Audio( StringRef const& path )
{
    out_audio.load_from_file( path );
}

float titian::Audio::duration() const
{
    return out_audio.duration_seconds();
}

float titian::Audio::sample( float time ) const
{
    return out_audio.sample_at_time( time );
}

titian::Ref<titian::Audio> titian::Audio::make_copy() const
{
    Ref audio = new Audio();
    audio->out_audio = this->out_audio;
    return audio;
}
