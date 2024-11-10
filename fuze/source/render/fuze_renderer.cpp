#include "fuze.h"


titian::FuzeRenderer::FuzeRenderer(
    StringRef const& filepath,
    kl::VideoType const& video_type,
    Int2 frame_size,
    int fps,
    int video_bit_rate,
    int audio_sample_rate )
    : m_path( filepath ), m_frame_size( frame_size ), m_fps( fps )
{
    m_video_writer = new kl::VideoWriter( filepath, video_type, frame_size, fps, video_bit_rate, audio_sample_rate );
}

titian::FuzeRenderer::FuzeRenderer(
    StringRef const& filepath,
    kl::AudioType audio_type,
    int audio_sample_rate )
    : m_path( filepath ), m_audio_type( audio_type )
{
    m_audio_writer = new kl::Audio( audio_sample_rate );
}

titian::FuzeRenderer::~FuzeRenderer()
{
    if ( m_video_writer )
        m_video_writer->finalize();
}

void titian::FuzeRenderer::load_tracks( Vector<Ref<Track>> const& tracks )
{
    for ( auto& track : tracks )
        this->tracks.push_back( track->make_copy() );

    for ( auto& track : this->tracks )
    {
        for ( auto& [_, media] : track->media )
        {
            if ( media->has_image() )
                media->image->cache_frame( this->m_frame_size );

            if ( media->has_video() )
                media->video->cache_scale = this->m_frame_size;
        }
    }
}

void titian::FuzeRenderer::load_audio()
{
    RAWAudio audio;
    if ( m_video_writer )
    {
        audio.sample_rate = m_video_writer->audio_sample_rate();
    }
    else if ( m_audio_writer )
    {
        audio.sample_rate = m_audio_writer->sample_rate;
    }
    audio.set_duration( end_time() );

    EffectPackage package;
    package.current_time = m_current_time;
    for ( auto& track : tracks )
    {
        for ( auto& [offset, media] : track->media )
        {
            package.media_start = offset;
            package.media_end = offset + media->duration;
            media->store_audio( package );
            kl::async_for( 0, (int) audio.size(), [&]( int i )
            {
                float time = audio.index_to_time( i ) - offset;
                audio[i] += media->out_audio.sample( time );
            } );
        }
    }

    if ( m_video_writer )
    {
        m_video_writer->add_audio( audio );
    }
    else if ( m_audio_writer )
    {
        m_audio_writer->insert( m_audio_writer->begin(), audio.begin(), audio.end() );
        m_audio_writer->save_to_file( m_path, m_audio_type );
        m_current_time = end_time();
    }
}

void titian::FuzeRenderer::load_frame()
{
    m_handler.prepare_frame( m_frame_size );

    EffectPackage package;
    package.current_time = m_current_time;
    for ( int i = int( tracks.size() ) - 1; i >= 0; i-- )
    {
        float offset = 0.0f;
        if ( Ref media = tracks[i]->get_media( m_current_time, offset ) )
        {
            package.media_start = offset;
            package.media_end = offset + media->duration;
            media->store_frame( package, true );
            m_handler.mix_frame( media->out_frame );
        }
    }

    if ( m_video_writer )
    {
        m_temp_image.resize( m_handler.out_frame.size() );
        m_handler.out_frame.retrieve( m_temp_image );
        m_video_writer->add_frame( m_temp_image );
    }

    m_current_time += 1.0f / (float) m_fps;
}

float titian::FuzeRenderer::progress() const
{
    return m_current_time / end_time();
}

float titian::FuzeRenderer::start_time() const
{
    if ( tracks.empty() )
        return 0.0f;

    float result = std::numeric_limits<float>::infinity();
    for ( auto& track : tracks )
    {
        for ( auto& [offset, _] : track->media )
            result = kl::min( result, offset );
    }
    return result;
}

float titian::FuzeRenderer::end_time() const
{
    float result = 0.0f;
    for ( auto& track : tracks )
    {
        for ( auto& [offset, media] : track->media )
            result = kl::max( result, offset + media->duration );
    }
    return result;
}
