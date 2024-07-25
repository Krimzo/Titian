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
kl::VideoReader::VideoReader(const std::string& filepath, const bool use_gpu)
{
    // Init
    ComPtr<IMFAttributes> attributes = nullptr;
    MFCreateAttributes(&attributes, 0) >> verify_result;

    attributes->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, TRUE) >> verify_result;
    attributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE) >> verify_result;

    if (use_gpu) {
        m_gpu = new GPU(IS_DEBUG, false, true);
        ComPtr<ID3D11Multithread> multithread;
        m_gpu->device().As(&multithread) >> verify_result;
        multithread->SetMultithreadProtected(TRUE);

        UINT reset_token = 0;
        ComPtr<IMFDXGIDeviceManager> manager;
        MFCreateDXGIDeviceManager(&reset_token, &manager) >> verify_result;
        manager->ResetDevice(m_gpu->device().Get(), reset_token) >> verify_result;
		attributes->SetUnknown(MF_SOURCE_READER_D3D_MANAGER, manager.Get()) >> verify_result;
    }

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

bool kl::VideoReader::get_frame(const float time, Image& out) const
{
    const INT64 time_100ns = INT64(time * 1e7);
    if (time_100ns < 0 || time_100ns > m_duration) {
		return false;
	}

    // Seek
    PROPVARIANT time_var;
    PropVariantInit(&time_var);
    time_var.vt = VT_I8;
    time_var.hVal.QuadPart = time_100ns;
    if (FAILED(m_reader->SetCurrentPosition(GUID_NULL, time_var))) {
        return false;
    }

    // Read sample
    ComPtr<IMFSample> sample;
    INT64 last_delta = std::numeric_limits<INT64>::max();
    while (true) {
        DWORD flags = NULL;
        LONGLONG time_stamp = 0;
        ComPtr<IMFSample> temp_sample;
        if (FAILED(m_reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, nullptr, &flags, &time_stamp, &temp_sample)) || !temp_sample) {
            return false;
        }
        const INT64 current_delta = std::abs(time_stamp - time_100ns);
        if (current_delta >= last_delta) {
            break;
        }
        last_delta = current_delta;
        sample = temp_sample;
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

    // Cleanup
	PropVariantClear(&time_var);
    return true;
}
