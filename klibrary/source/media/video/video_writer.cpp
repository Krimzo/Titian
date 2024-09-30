#include "klibrary.h"


static const int _media_init = []
{
    constexpr DWORD init_args = COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY;
    CoInitializeEx(nullptr, init_args) >> kl::verify_result;
    MFStartup(MF_VERSION) >> kl::verify_result;
    return 0;
}();

kl::VideoWriter::VideoWriter(const std::string_view& filepath, const VideoType& video_type, const Int2 frame_size, const int fps, const int video_bit_rate, const int audio_sample_rate)
    : m_width(frame_size.x)
    , m_height(frame_size.y)
    , m_fps(fps)
    , m_bit_rate(video_bit_rate)
    , m_sample_rate(audio_sample_rate)
    , m_frame_duration(10'000'000 / m_fps)
{
    const std::wstring converted_path = convert_string(filepath);
    MFCreateSinkWriterFromURL(converted_path.data(), nullptr, nullptr, &m_writer) >> verify_result;

    ComRef<IMFMediaType> video_out_type;
    MFCreateMediaType(&video_out_type) >> verify_result;

    video_out_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video) >> verify_result;
    video_out_type->SetGUID(MF_MT_SUBTYPE, video_type.type()) >> verify_result;
    if (video_type.profile() > 0) {
        video_out_type->SetUINT32(MF_MT_MPEG2_PROFILE, video_type.profile()) >> verify_result;
    }
    video_out_type->SetUINT32(MF_MT_AVG_BITRATE, m_bit_rate) >> verify_result;
    video_out_type->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive) >> verify_result;
    MFSetAttributeSize(video_out_type.get(), MF_MT_FRAME_SIZE, m_width, m_height) >> verify_result;
    MFSetAttributeRatio(video_out_type.get(), MF_MT_FRAME_RATE, m_fps, 1) >> verify_result;
    m_writer->AddStream(video_out_type.get(), &m_video_index) >> verify_result;

    ComRef<IMFMediaType> video_in_type;
    MFCreateMediaType(&video_in_type) >> verify_result;

    video_in_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video) >> verify_result;
    video_in_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32) >> verify_result;
    video_in_type->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive) >> verify_result;
    MFSetAttributeSize(video_in_type.get(), MF_MT_FRAME_SIZE, m_width, m_height) >> verify_result;
    MFSetAttributeRatio(video_in_type.get(), MF_MT_FRAME_RATE, m_fps, 1) >> verify_result;
    m_writer->SetInputMediaType(m_video_index, video_in_type.get(), nullptr) >> verify_result;

    if (m_sample_rate > 0) {
        ComRef<IMFMediaType> audio_out_type;
        MFCreateMediaType(&audio_out_type) >> verify_result;

        audio_out_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio) >> verify_result;
        audio_out_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_AAC) >> verify_result;
        audio_out_type->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 1) >> verify_result;
        audio_out_type->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16) >> verify_result;
        audio_out_type->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, m_sample_rate) >> verify_result;
        m_writer->AddStream(audio_out_type.get(), &m_audio_index) >> verify_result;

        ComRef<IMFMediaType> audio_in_type;
        MFCreateMediaType(&audio_in_type) >> verify_result;

        audio_in_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio) >> verify_result;
        audio_in_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_Float) >> verify_result;
        audio_in_type->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 1) >> verify_result;
        audio_in_type->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 32) >> verify_result;
        audio_in_type->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, 4) >> verify_result;
        audio_in_type->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, m_sample_rate) >> verify_result;
        audio_in_type->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, m_sample_rate * sizeof(float)) >> verify_result;
        m_writer->SetInputMediaType(m_audio_index, audio_in_type.get(), nullptr) >> verify_result;
    }

    m_writer->BeginWriting() >> verify_result;
}

kl::Int2 kl::VideoWriter::frame_size() const
{
    return { (int) m_width, (int) m_height };
}

int kl::VideoWriter::fps() const
{
    return (int) m_fps;
}

int kl::VideoWriter::video_bit_rate() const
{
    return (int) m_bit_rate;
}

int kl::VideoWriter::frame_count() const
{
    return (int) (m_video_time / m_frame_duration);
}

bool kl::VideoWriter::add_frame(const Image& frame)
{
    if (frame.width() != m_width || frame.height() != m_height) {
        return false;
    }

    const int frame_byte_width = m_width * sizeof(RGB);
    const int frame_byte_size = frame_byte_width * m_height;

    ComRef<IMFMediaBuffer> media_buffer;
    MFCreateMemoryBuffer(frame_byte_size, &media_buffer) >> verify_result;
    media_buffer->SetCurrentLength(frame_byte_size) >> verify_result;

    ComRef<IMFSample> media_sample;
    MFCreateSample(&media_sample) >> verify_result;
    media_sample->AddBuffer(media_buffer.get()) >> verify_result;
    media_sample->SetSampleDuration((LONGLONG) m_frame_duration) >> verify_result;

    RGB* out_buffer = nullptr;
    const RGB* in_buffer = frame.ptr();
    media_buffer->Lock((BYTE**) &out_buffer, nullptr, nullptr) >> verify_result;
    for (uint32_t y = 0; y < m_height; y++) {
        copy<byte>(out_buffer + (m_height - 1 - y) * m_width, in_buffer + y * m_width, frame_byte_width);
    }
    media_buffer->Unlock() >> verify_result;

    if (FAILED(media_sample->SetSampleTime((LONGLONG) video_duration_100ns()))) {
        return false;
    }
    if (FAILED(m_writer->WriteSample(m_video_index, media_sample.get()))) {
        return false;
    }
    m_video_time += m_frame_duration;
    return true;
}

uint64_t kl::VideoWriter::video_duration_100ns() const
{
    return m_video_time;
}

float kl::VideoWriter::video_duration_seconds() const
{
    static constexpr float diver = 1.0f / 1e7f;
    return video_duration_100ns() * diver;
}

int kl::VideoWriter::audio_sample_rate() const
{
    return (int) m_sample_rate;
}

bool kl::VideoWriter::add_audio(const Audio& audio)
{
    if (m_sample_rate <= 0 || audio.sample_rate != m_sample_rate || audio.empty()) {
        return false;
    }

    const int sample_byte_size = (int) audio.size() * sizeof(float);
    ComRef<IMFMediaBuffer> media_buffer;
    MFCreateMemoryBuffer(sample_byte_size, &media_buffer) >> verify_result;
    media_buffer->SetCurrentLength(sample_byte_size) >> verify_result;

    ComRef<IMFSample> media_sample;
    MFCreateSample(&media_sample) >> verify_result;
    media_sample->AddBuffer(media_buffer.get()) >> verify_result;
    media_sample->SetSampleDuration(audio.duration_100ns()) >> verify_result;

    BYTE* out_buffer = nullptr;
    media_buffer->Lock(&out_buffer, nullptr, nullptr) >> verify_result;
    copy<byte>(out_buffer, audio.data(), sample_byte_size);
    media_buffer->Unlock() >> verify_result;

    if (FAILED(media_sample->SetSampleTime((LONGLONG) audio_duration_100ns()))) {
        return false;
    }
    if (FAILED(m_writer->WriteSample(m_audio_index, media_sample.get()))) {
        return false;
    }
    m_audio_time += audio.duration_100ns();
    return true;
}

uint64_t kl::VideoWriter::audio_duration_100ns() const
{
    return m_audio_time;
}

float kl::VideoWriter::audio_duration_seconds() const
{
    static constexpr float diver = 1.0f / 1e7f;
    return audio_duration_100ns() * diver;
}

void kl::VideoWriter::finalize() const
{
    m_writer->Finalize() >> verify_result;
}
