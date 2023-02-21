#include "media/video/video_reader.h"

#include "media/media.h"
#include "utility/utility.h"

using namespace kl::media_utility;


// Utility
static void configure_reader(ComPtr<IMFSourceReader> reader)
{
    ComPtr<IMFMediaType> media_type = nullptr;
    fail_check_(reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &media_type), "Failed to get default video type");

    GUID major_type = {};
    fail_check_(media_type->GetGUID(MF_MT_MAJOR_TYPE, &major_type), "Failed to get major video type");

    ComPtr<IMFMediaType> new_type = nullptr;
    fail_check_(MFCreateMediaType(&new_type), "Failed to create new video type");
    fail_check_(new_type->SetGUID(MF_MT_MAJOR_TYPE, major_type), "Failed to set major video type");
    fail_check_(new_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32), "Failed to set sub video type");
    fail_check_(reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, new_type.Get()), "Failed to set video type");
}

static uint64_t get_video_byte_size(ComPtr<IMFSourceReader> reader)
{
    PROPVARIANT variant = {};
    if (!succeeded_(reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_TOTAL_FILE_SIZE, &variant))) {
        return 0;
    }

    uint64_t byte_size = 0;
    PropVariantToUInt64(variant, &byte_size);
    PropVariantClear(&variant);
    return byte_size;
}

static int64_t get_video_duration_100ns(ComPtr<IMFSourceReader> reader)
{
    PROPVARIANT variant = {};
    if (!succeeded_(reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &variant))) {
        return 0;
    }

    int64_t duration = 0;
    PropVariantToInt64(variant, &duration);
    PropVariantClear(&variant);
    return duration;
}

static kl::int2 get_video_frame_size(ComPtr<IMFSourceReader> reader)
{
    ComPtr<IMFMediaType> current_type = nullptr;
    reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current_type);
    if (!current_type) {
        return {};
    }

    kl::int2 frame_size = {};
    MFGetAttributeSize(current_type.Get(), MF_MT_FRAME_SIZE, (UINT32*) &frame_size.x, (UINT32*) &frame_size.y);
    return frame_size;
}

static float get_video_fps(ComPtr<IMFSourceReader> reader)
{
    ComPtr<IMFMediaType> current_type = nullptr;
    reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current_type);
    if (!current_type) {
        return 0.0f;
    }

    UINT attribute1 = 0, attribute2 = 0;
    MFGetAttributeRatio(current_type.Get(), MF_MT_FRAME_RATE, &attribute1, &attribute2);

    return (float) attribute1 / attribute2;
}

// Video reader
kl::video_reader::video_reader(const std::string& filepath)
{
    // Init
    ComPtr<IMFAttributes> attributes = nullptr;
    fail_check_(MFCreateAttributes(&attributes, 1), "Failed to create attributes");

    fail_check_(attributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, true), "Failed to enable video processing");

    const std::wstring converted_path = strings::to_w_string(filepath);
    fail_check_(MFCreateSourceReaderFromURL(converted_path.c_str(), attributes.Get(), &reader_), "Failed to create SourceReader");
    configure_reader(reader_);

    // Getting info
    byte_size_ = get_video_byte_size(reader_);
    duration_ = get_video_duration_100ns(reader_);

    frame_size_ = get_video_frame_size(reader_);
    frame_byte_size_ = frame_size_.x * frame_size_.y * 4;

    fps_ = get_video_fps(reader_);
    frame_count_ = int(duration_seconds() * fps_);
}

kl::video_reader::~video_reader()
{}

size_t kl::video_reader::byte_size() const
{
    return byte_size_;
}

int64_t kl::video_reader::duration_100ns() const
{
    return duration_;
}

float kl::video_reader::duration_seconds() const
{
    static constexpr float diver = 1.0f / 1e7f;
    return duration_ * diver;
}

kl::int2 kl::video_reader::frame_size() const
{
    return frame_size_;
}

int kl::video_reader::frame_count() const
{
    return frame_count_;
}

float kl::video_reader::fps() const
{
    return fps_;
}

bool kl::video_reader::get_next_frame(image& out) const
{
    // Read sample
    DWORD flags = NULL;
    LONGLONG time_stamp = 0;
    ComPtr<IMFSample> sample = nullptr;

    if (!succeeded_(reader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, nullptr, &flags, &time_stamp, &sample)) || !sample) {
        return false;
    }

    // Convert to array
    ComPtr<IMFMediaBuffer> media_buffer = nullptr;
    if (!succeeded_(sample->ConvertToContiguousBuffer(&media_buffer)) || !media_buffer) {
        return false;
    }

    // Copy data
    BYTE* frame_data = nullptr;
    DWORD frame_byte_size = 0;
    fail_check_(media_buffer->Lock(&frame_data, nullptr, &frame_byte_size), "Failed to lock the bytes [video_reader]");

    out.resize(frame_size_);
    const size_t pixel_count = (size_t) out.width() * out.height();
    const color* frame_source = (color*) frame_data;
    color* frame_target = out;

    for (size_t i = 0; i < pixel_count; i++) {
        frame_target[i].r = frame_source[i].r;
        frame_target[i].g = frame_source[i].g;
        frame_target[i].b = frame_source[i].b;
    }

    // Cleanup
    fail_check_(media_buffer->Unlock(), "Failed to unlock bytes [video_reader]");
    return true;
}
