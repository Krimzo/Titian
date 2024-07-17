#include "klibrary.h"


using kl::ComPtr;

// Utility
static void configure_reader(const ComPtr<IMFSourceReader>& reader)
{
    ComPtr<IMFMediaType> media_type = nullptr;
    reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &media_type) >> kl::verify_result;

    GUID major_type = {};
    media_type->GetGUID(MF_MT_MAJOR_TYPE, &major_type) >> kl::verify_result;

    ComPtr<IMFMediaType> new_type = nullptr;
    MFCreateMediaType(&new_type) >> kl::verify_result;
    new_type->SetGUID(MF_MT_MAJOR_TYPE, major_type) >> kl::verify_result;
    new_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32) >> kl::verify_result;
    reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, new_type.Get()) >> kl::verify_result;
}

static uint64_t video_byte_size(const ComPtr<IMFSourceReader>& reader)
{
    PROPVARIANT variant{};
    if (FAILED(reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_TOTAL_FILE_SIZE, &variant))) {
        return 0;
    }

    uint64_t byte_size = 0;
    PropVariantToUInt64(variant, &byte_size);
    PropVariantClear(&variant);
    return byte_size;
}

static int64_t video_duration_100ns(const ComPtr<IMFSourceReader>& reader)
{
    PROPVARIANT variant = {};
    if (FAILED(reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &variant))) {
        return 0;
    }

    int64_t duration = 0;
    PropVariantToInt64(variant, &duration);
    PropVariantClear(&variant);
    return duration;
}

static kl::Int2 video_frame_size(const ComPtr<IMFSourceReader>& reader)
{
    ComPtr<IMFMediaType> current_type = nullptr;
    reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current_type);
    if (!current_type) {
        return {};
    }

    kl::Int2 frame_size = {};
    MFGetAttributeSize(current_type.Get(), MF_MT_FRAME_SIZE, (UINT32*) &frame_size.x, (UINT32*) &frame_size.y);
    return frame_size;
}

static float video_fps(const ComPtr<IMFSourceReader>& reader)
{
    ComPtr<IMFMediaType> current_type = nullptr;
    reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current_type);
    if (!current_type) {
        return 0.0f;
    }

    UINT attribute1 = 0, attribute2 = 0;
    MFGetAttributeRatio(current_type.Get(), MF_MT_FRAME_RATE, &attribute1, &attribute2);

    return (float) attribute1 / (float) attribute2;
}

// Video reader
kl::VideoReader::VideoReader(const std::string& filepath)
{
    // Init
    ComPtr<IMFAttributes> attributes = nullptr;
    MFCreateAttributes(&attributes, 1) >> verify_result;

    attributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, true) >> verify_result;

    const std::wstring converted_path = convert_string(filepath);
    MFCreateSourceReaderFromURL(converted_path.c_str(), attributes.Get(), &m_reader) >> verify_result;
    configure_reader(m_reader);

    // Getting info
    m_byte_size = video_byte_size(m_reader);
    m_duration = video_duration_100ns(m_reader);

    m_frame_size = video_frame_size(m_reader);
    m_frame_byte_size = m_frame_size.x * m_frame_size.y * 4;

    m_fps = video_fps(m_reader);
    m_frame_count = (int) (duration_seconds() * m_fps);
}

uint64_t kl::VideoReader::byte_size() const
{
    return m_byte_size;
}

int64_t kl::VideoReader::duration_100ns() const
{
    return m_duration;
}

float kl::VideoReader::duration_seconds() const
{
    static constexpr float diver = 1.0f / 1e7f;
    return m_duration * diver;
}

kl::Int2 kl::VideoReader::frame_size() const
{
    return m_frame_size;
}

int kl::VideoReader::frame_count() const
{
    return m_frame_count;
}

float kl::VideoReader::fps() const
{
    return m_fps;
}

bool kl::VideoReader::next_frame(Image& out) const
{
    // Read sample
    DWORD flags = NULL;
    LONGLONG time_stamp = 0;
    ComPtr<IMFSample> sample;
    if (FAILED(m_reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, nullptr, &flags, &time_stamp, &sample)) || !sample) {
        return false;
    }

    // Convert to array
    ComPtr<IMFMediaBuffer> media_buffer;
    if (FAILED(sample->ConvertToContiguousBuffer(&media_buffer)) || !media_buffer) {
        return false;
    }

    // Copy data
    BYTE* frame_data = nullptr;
    DWORD frame_byte_size = 0;
    media_buffer->Lock(&frame_data, nullptr, &frame_byte_size) >> verify_result;

    out.resize(m_frame_size);
    const size_t pixel_count = (size_t) out.width() * out.height();
    const Color* frame_source = (Color*) frame_data;
    Color* frame_target = out;

    for (size_t i = 0; i < pixel_count; i++) {
        frame_target[i].r = frame_source[i].r;
        frame_target[i].g = frame_source[i].g;
        frame_target[i].b = frame_source[i].b;
    }

    media_buffer->Unlock() >> verify_result;
    return true;
}
