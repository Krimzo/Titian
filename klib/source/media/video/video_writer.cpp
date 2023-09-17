#include "klib.h"

using namespace kl::media_utility;


kl::VideoWriter::VideoWriter(const std::string& filepath, const kl::Int2& frame_size, const int fps, const int bit_rate, const GUID& encoding_format)
    : m_width(frame_size.x), m_height(frame_size.y), m_fps(fps), m_bit_rate(bit_rate), m_encoding_format(encoding_format),
    m_pixel_count(m_width * m_height), m_frame_duration(10'000'000 / m_fps),
    m_frame_byte_width(4 * m_width), m_frame_byte_size(m_frame_byte_width * m_height)
{
    // Sink-Writer
    const std::wstring converted_path = convert_string(filepath);
    fail_check_(MFCreateSinkWriterFromURL(converted_path.c_str(), nullptr, nullptr, &m_sink_writer), "Failed to create SinkWriter");

    // Output media type
    Microsoft::WRL::ComPtr<IMFMediaType> media_type_out = nullptr;
    fail_check_(MFCreateMediaType(&media_type_out), "Failed to create output media type");

    // Output media type info
    fail_check_(media_type_out->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video), "Failed to set major type [output media]");
    fail_check_(media_type_out->SetGUID(MF_MT_SUBTYPE, m_encoding_format), "Failed to set subtype [output media]");
    fail_check_(media_type_out->SetUINT32(MF_MT_AVG_BITRATE, m_bit_rate), "Failed to set average bitrate [output media]");
    fail_check_(media_type_out->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive), "Failed to set interlace mode [output media]");
    fail_check_(MFSetAttributeSize(media_type_out.Get(), MF_MT_FRAME_SIZE, m_width, m_height), "Failed to set frame size [output media]");
    fail_check_(MFSetAttributeRatio(media_type_out.Get(), MF_MT_FRAME_RATE, m_fps, 1), "Failed to set FPS [output media]");
    fail_check_(MFSetAttributeRatio(media_type_out.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1), "Failed to set pixel aspect ratio [output media]");
    fail_check_(m_sink_writer->AddStream(media_type_out.Get(), &m_stream_index), "Failed to add output media type stream");

    // Input media type
    Microsoft::WRL::ComPtr<IMFMediaType> media_type_in = nullptr;
    fail_check_(MFCreateMediaType(&media_type_in), "Failed to create input media type");

    // Input media type info
    fail_check_(media_type_in->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video), "Failed to set major type [input media]");
    fail_check_(media_type_in->SetGUID(MF_MT_SUBTYPE, m_input_format), "Failed to set subtype [input media]");
    fail_check_(media_type_in->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive), "Failed to set interlace mode [input media]");
    fail_check_(MFSetAttributeSize(media_type_in.Get(), MF_MT_FRAME_SIZE, m_width, m_height), "Failed to set frame size [input media]");
    fail_check_(MFSetAttributeRatio(media_type_in.Get(), MF_MT_FRAME_RATE, m_fps, 1), "Failed to set FPS [input media]");
    fail_check_(MFSetAttributeRatio(media_type_in.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1), "Failed to set pixel aspect ratio [input media]");
    fail_check_(m_sink_writer->SetInputMediaType(m_stream_index, media_type_in.Get(), nullptr), "Failed to add input media type stream");

    // Setup buffers
    fail_check_(MFCreateMemoryBuffer(m_frame_byte_size, &m_media_buffer), "Failed to create media buffer");
    fail_check_(m_media_buffer->SetCurrentLength(m_frame_byte_size), "Failed to set media buffer length");

    fail_check_(MFCreateSample(&m_media_sample), "Failed to create media sample");
    fail_check_(m_media_sample->AddBuffer(m_media_buffer.Get()), "Failed to add media buffer to media sample");
    fail_check_(m_media_sample->SetSampleDuration((LONGLONG) m_frame_duration), "Failed to set sample frame duration");

    // Start accepting data
    fail_check_(m_sink_writer->BeginWriting(), "Failed to start accepting frames");
}

kl::Int2 kl::VideoWriter::frame_size() const
{
    return { (int) m_width, (int) m_height };
}

int kl::VideoWriter::fps() const
{
    return (int) m_fps;
}

int kl::VideoWriter::bit_rate() const
{
    return (int) m_bit_rate;
}

GUID kl::VideoWriter::format() const
{
    return m_encoding_format;
}

bool kl::VideoWriter::add_frame(const Image& frame)
{
    const Int2 frame_size = this->frame_size();
    if (frame.width() != frame_size.x || frame.height() != frame_size.y) {
        return false;
    }

    const kl::Color* in_buffer = frame;
    kl::Color* out_buffer = nullptr;

    fail_check_(m_media_buffer->Lock((BYTE**) &out_buffer, nullptr, nullptr), "Failed to lock the bytes [video_writer]");
    for (uint32_t y = 0; y < m_height; y++) {
        memcpy(out_buffer + (m_height - 1 - y) * m_width, in_buffer + y * m_width, m_frame_byte_width);
    }
    fail_check_(m_media_buffer->Unlock(), "Failed to unlock bytes [video_writer]");

    if (!succeeded_(m_media_sample->SetSampleTime((LONGLONG) duration_100ns()))) {
        return false;
    }

    if (!succeeded_(m_sink_writer->WriteSample(m_stream_index, m_media_sample.Get()))) {
        return false;
    }

    m_frame_counter += 1;
    return true;
}

int kl::VideoWriter::frame_count() const
{
    return m_frame_counter;
}

uint64_t kl::VideoWriter::duration_100ns() const
{
    return m_frame_counter * m_frame_duration;
}

float kl::VideoWriter::duration_seconds() const
{
    static constexpr float diver = 1.0f / 1e7f;
    return duration_100ns() * diver;
}

bool kl::VideoWriter::finalize() const
{
    return succeeded_(m_sink_writer->Finalize());
}
