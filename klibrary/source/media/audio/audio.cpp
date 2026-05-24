#include "klibrary.h"


void kl::AudioSample::set_mono( float value )
{
    left = value;
    right = value;
}

void kl::AudioSample::adjust_volume( float amount )
{
    left *= amount;
    right *= amount;
}

kl::Audio::Audio( int sample_rate )
    : sample_rate( sample_rate )
{
}

kl::Audio::Audio( std::string_view const& path )
{
    load_from_file( path );
}

kl::Audio::Audio( std::wstring_view const& path )
{
    load_from_file( path );
}

uint64_t kl::Audio::duration_100ns() const
{
    return size() * 10'000'000 / sample_rate;
}

float kl::Audio::duration_seconds() const
{
    return size() / (float) sample_rate;
}

void kl::Audio::set_duration( float duration )
{
    resize( size_t( sample_rate * duration ) );
}

void kl::Audio::adjust_volume( float amount )
{
    for ( auto& sample : *this )
        sample.adjust_volume( amount );
}

float kl::Audio::index_to_time( int at_index ) const
{
    return at_index / (float) sample_rate;
}

int kl::Audio::time_to_index( float at_time ) const
{
    return int( at_time * sample_rate );
}

kl::AudioSample kl::Audio::sample_at_index( int index ) const
{
    if ( index >= 0 && index < (int) size() )
        return ( *this )[index];
    else
        return {};
}

kl::AudioSample kl::Audio::sample_at_time( float time ) const
{
    const int first_index = time_to_index( time );
    const int second_index = first_index + 1;
    const float first_time = index_to_time( first_index );
    const float second_time = index_to_time( second_index );
    const AudioSample first_sample = sample_at_index( first_index );
    const AudioSample second_sample = sample_at_index( second_index );
    return AudioSample{
        .left = lerp( unlerp( time, first_time, second_time ), first_sample.left, second_sample.left ),
        .right = lerp( unlerp( time, first_time, second_time ), first_sample.right, second_sample.right ),
    };
}

bool kl::Audio::load_from_memory( void const* data, uint64_t byte_size )
{
    HRESULT hr = 0;

    ComRef<IStream> stream{ SHCreateMemStream( reinterpret_cast<BYTE const*>( data ), UINT( byte_size ) ) };
    if ( !stream )
        return false;

    ComRef<IMFByteStream> byte_stream;
    hr = MFCreateMFByteStreamOnStream( stream.get(), &byte_stream );
    if ( FAILED( hr ) )
        return false;

    ComRef<IMFSourceReader> reader;
    hr = MFCreateSourceReaderFromByteStream( byte_stream.get(), nullptr, &reader );
    if ( FAILED( hr ) )
        return false;

    ComRef<IMFMediaType> media_type;
    hr = reader->GetCurrentMediaType( MF_SOURCE_READER_FIRST_AUDIO_STREAM, &media_type );
    if ( FAILED( hr ) )
        return false;

    media_type->GetUINT32( MF_MT_AUDIO_SAMPLES_PER_SECOND, (UINT32*) &sample_rate );

    ComRef<IMFMediaType> new_media_type;
    hr = MFCreateMediaType( &new_media_type );
    if ( FAILED( hr ) )
        return false;

    new_media_type->SetGUID( MF_MT_MAJOR_TYPE, MFMediaType_Audio ) >> verify_result;
    new_media_type->SetGUID( MF_MT_SUBTYPE, MFAudioFormat_Float ) >> verify_result;
    new_media_type->SetUINT32( MF_MT_AUDIO_NUM_CHANNELS, sizeof( AudioSample ) / sizeof( float ) ) >> verify_result;
    new_media_type->SetUINT32( MF_MT_AUDIO_SAMPLES_PER_SECOND, sample_rate ) >> verify_result;
    new_media_type->SetUINT32( MF_MT_AUDIO_BITS_PER_SAMPLE, sizeof( float ) * 8 ) >> verify_result;
    new_media_type->SetUINT32( MF_MT_AUDIO_BLOCK_ALIGNMENT, sizeof( AudioSample ) ) >> verify_result;
    new_media_type->SetUINT32( MF_MT_AUDIO_AVG_BYTES_PER_SECOND, sample_rate * sizeof( AudioSample ) ) >> verify_result;

    hr = reader->SetCurrentMediaType( MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, new_media_type.get() );
    if ( FAILED( hr ) )
        return false;

    clear();
    while ( true )
    {
        DWORD flags = NULL;
        LONGLONG time_stamp = 0;
        ComRef<IMFSample> sample;
        if ( FAILED( reader->ReadSample( MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, nullptr, &flags, &time_stamp, &sample ) ) || !sample )
            break;

        resize( time_stamp * sample_rate / 10'000'000LL );

        ComRef<IMFMediaBuffer> media_buffer;
        if ( FAILED( sample->ConvertToContiguousBuffer( &media_buffer ) ) || !media_buffer )
            break;

        BYTE* sample_data = nullptr;
        DWORD sample_byte_size = 0;
        media_buffer->Lock( &sample_data, nullptr, &sample_byte_size ) >> verify_result;

        const int old_size = (int) size();
        resize( old_size + ( sample_byte_size / sizeof( AudioSample ) ) );
        copy<byte>( this->data() + old_size, sample_data, sample_byte_size );
    }
    return true;
}

bool kl::Audio::load_from_buffer( std::string_view const& buffer )
{
    return load_from_memory( buffer.data(), buffer.size() );
}

bool kl::Audio::load_from_file( std::string_view const& filepath )
{
    return load_from_buffer( read_file_string( filepath ) );
}

bool kl::Audio::load_from_file( std::wstring_view const& filepath )
{
    return load_from_buffer( read_file_string( filepath ) );
}

bool kl::Audio::save_to_buffer( std::string& buffer, AudioType type ) const
{
    HRESULT hr = 0;

    ComRef<IStream> stream{ SHCreateMemStream( nullptr, 0 ) };
    if ( !stream )
        return false;

    ComRef<IMFByteStream> byte_stream;
    hr = MFCreateMFByteStreamOnStream( stream.get(), &byte_stream );
    if ( FAILED( hr ) )
        return false;

    ComRef<IMFMediaSink> sink;
    if ( type == AudioType::MP3 )
    {
        MFCreateMP3MediaSink( byte_stream.get(), &sink ) >> verify_result;
    }
    else
    {
        return false;
    }

    ComRef<IMFSinkWriter> writer;
    hr = MFCreateSinkWriterFromMediaSink( sink.get(), nullptr, &writer );
    if ( FAILED( hr ) )
        return false;

    ComRef<IMFMediaType> input_type;
    hr = MFCreateMediaType( &input_type );
    if ( FAILED( hr ) )
        return false;

    input_type->SetGUID( MF_MT_MAJOR_TYPE, MFMediaType_Audio ) >> verify_result;
    input_type->SetGUID( MF_MT_SUBTYPE, MFAudioFormat_Float ) >> verify_result;
    input_type->SetUINT32( MF_MT_AUDIO_NUM_CHANNELS, sizeof( AudioSample ) / sizeof( float ) ) >> verify_result;
    input_type->SetUINT32( MF_MT_AUDIO_SAMPLES_PER_SECOND, sample_rate ) >> verify_result;
    input_type->SetUINT32( MF_MT_AUDIO_BITS_PER_SAMPLE, sizeof( float ) * 8 ) >> verify_result;
    input_type->SetUINT32( MF_MT_AUDIO_BLOCK_ALIGNMENT, sizeof( AudioSample ) ) >> verify_result;
    input_type->SetUINT32( MF_MT_AUDIO_AVG_BYTES_PER_SECOND, sample_rate * sizeof( AudioSample ) ) >> verify_result;
    writer->SetInputMediaType( 0, input_type.get(), nullptr ) >> verify_result;

    writer->BeginWriting() >> verify_result;
    for ( int i = 0; i < (int) size();)
    {
        const int sample_count = min( sample_rate, (int) size() - i );
        const int sample_byte_size = sample_count * sizeof( AudioSample );

        ComRef<IMFMediaBuffer> media_buffer;
        MFCreateMemoryBuffer( sample_byte_size, &media_buffer ) >> verify_result;
        media_buffer->SetCurrentLength( sample_byte_size ) >> verify_result;

        ComRef<IMFSample> media_sample;
        MFCreateSample( &media_sample ) >> verify_result;
        media_sample->AddBuffer( media_buffer.get() ) >> verify_result;

        BYTE* out_buffer = nullptr;
        media_buffer->Lock( &out_buffer, nullptr, nullptr ) >> verify_result;
        copy<byte>( out_buffer, data() + i, sample_byte_size );
        media_buffer->Unlock() >> verify_result;

        media_sample->SetSampleDuration( ( sample_count * 10'000'000LL ) / sample_rate ) >> verify_result;
        media_sample->SetSampleTime( ( i * 10'000'000LL ) / sample_rate ) >> verify_result;
        writer->WriteSample( 0, media_sample.get() ) >> verify_result;
        i += sample_count;
    }
    writer->Finalize() >> verify_result;

    QWORD length = 0;
    byte_stream->GetLength( &length ) >> verify_result;
    buffer.resize( (size_t) length );
    byte_stream->SetCurrentPosition( 0 ) >> verify_result;
    ULONG ignored = 0;
    byte_stream->Read( (BYTE*) buffer.data(), (ULONG) buffer.size(), &ignored ) >> verify_result;
    return true;
}

bool kl::Audio::save_to_file( std::string_view const& filepath, AudioType type ) const
{
    std::string buffer;
    if ( save_to_buffer( buffer, type ) )
        return write_file_string( filepath, buffer );
    return false;
}

bool kl::Audio::save_to_file( std::wstring_view const& filepath, AudioType type ) const
{
    std::string buffer;
    if ( save_to_buffer( buffer, type ) )
        return write_file_string( filepath, buffer );
    return false;
}
