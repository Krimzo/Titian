#include "klibrary.h"


using kl::ComRef;

static void configure_reader(const ComRef<IMFSourceReader>& reader, const kl::Int2 size)
{
    ComRef<IMFMediaType> media_type;
    reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &media_type) >> kl::verify_result;

    GUID major_type = {};
    media_type->GetGUID(MF_MT_MAJOR_TYPE, &major_type) >> kl::verify_result;

    ComRef<IMFMediaType> new_type;
    MFCreateMediaType(&new_type) >> kl::verify_result;

    new_type->SetGUID(MF_MT_MAJOR_TYPE, major_type) >> kl::verify_result;
    new_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32) >> kl::verify_result;
    if (size.x > 0 && size.y > 0) {
        MFSetAttributeSize(new_type.get(), MF_MT_FRAME_SIZE, (UINT32) size.x, (UINT32) size.y) >> kl::verify_result;
    }

    reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, new_type.get()) >> kl::verify_result;
}

static uint64_t video_byte_size(const ComRef<IMFSourceReader>& reader)
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

static int64_t video_duration_100ns(const ComRef<IMFSourceReader>& reader)
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

static kl::Int2 video_frame_size(const ComRef<IMFSourceReader>& reader)
{
    ComRef<IMFMediaType> current_type;
    reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current_type);
    if (!current_type) {
        return {};
    }

    kl::Int2 frame_size = {};
    MFGetAttributeSize(current_type.get(), MF_MT_FRAME_SIZE, (UINT32*) &frame_size.x, (UINT32*) &frame_size.y);
    return frame_size;
}

static float video_fps(const ComRef<IMFSourceReader>& reader)
{
    ComRef<IMFMediaType> current_type;
    reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current_type);
    if (!current_type) {
        return 0.0f;
    }

    UINT attribute1 = 0, attribute2 = 0;
    MFGetAttributeRatio(current_type.get(), MF_MT_FRAME_RATE, &attribute1, &attribute2);

    return (float) attribute1 / (float) attribute2;
}

kl::VideoReader::VideoReader(const std::string_view& filepath, const Int2 output_size, const bool use_gpu)
{
    ComRef<IMFAttributes> attributes;
    MFCreateAttributes(&attributes, 0) >> verify_result;

    attributes->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, TRUE) >> verify_result;
    attributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE) >> verify_result;

    if (use_gpu) {
        m_gpu = new VideoGPU();
        ComRef<ID3D11Multithread> multithread;
        m_gpu->device().as(multithread) >> verify_result;
        multithread->SetMultithreadProtected(TRUE);

        UINT reset_token = 0;
        ComRef<IMFDXGIDeviceManager> manager;
        MFCreateDXGIDeviceManager(&reset_token, &manager) >> verify_result;
        manager->ResetDevice(m_gpu->device().get(), reset_token) >> verify_result;
		attributes->SetUnknown(MF_SOURCE_READER_D3D_MANAGER, manager.get()) >> verify_result;
    }

    const std::wstring converted_path = convert_string(filepath);
    MFCreateSourceReaderFromURL(converted_path.data(), attributes.get(), &m_reader) >> verify_result;
    configure_reader(m_reader, output_size);

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

bool kl::VideoReader::seek(const float time) const
{
    PROPVARIANT time_var;
    PropVariantInit(&time_var);
    time_var.vt = VT_I8;
    time_var.hVal.QuadPart = LONGLONG(time * 1e7);
    return SUCCEEDED(m_reader->SetCurrentPosition(GUID_NULL, time_var));
}

bool kl::VideoReader::read_frame(Image& out, int* out_index) const
{
    DWORD flags = NULL;
    LONGLONG time_stamp = 0;
    ComRef<IMFSample> sample;
    if (FAILED(m_reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, nullptr, &flags, &time_stamp, &sample)) || !sample) {
        return false;
    }

    ComRef<IMFMediaBuffer> media_buffer;
    if (FAILED(sample->ConvertToContiguousBuffer(&media_buffer)) || !media_buffer) {
        return false;
    }

    BYTE* frame_data = nullptr;
    DWORD frame_byte_size = 0;
    media_buffer->Lock(&frame_data, nullptr, &frame_byte_size) >> verify_result;

    out.resize(m_frame_size);
    const RGB* frame_source = (RGB*) frame_data;
    RGB* frame_target = out.ptr();

    kl::async_for(0, out.width() * out.height(), [&](const int i)
    {
        frame_target[i].r = frame_source[i].r;
        frame_target[i].g = frame_source[i].g;
        frame_target[i].b = frame_source[i].b;
    });
	if (out_index) {
		*out_index = int(time_stamp * 1e-7 * m_fps);
	}
    return true;
}
