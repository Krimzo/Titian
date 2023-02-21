#include "media/video/video_writer.h"

#include "media/media.h"
#include "utility/utility.h"

using namespace kl::media_utility;


kl::video_writer::video_writer(const std::string& filepath, const kl::int2& frame_size, int fps, int bit_rate, const GUID& encoding_format)
    : width_(frame_size.x), height_(frame_size.y), fps_(fps), bit_rate_(bit_rate), encoding_format_(encoding_format),
    pixel_count_(width_* height_), frame_duration_(10'000'000 / fps_),
    frame_byte_width_(4 * width_), frame_byte_size_(frame_byte_width_* height_)
{
    // Sink-Writer
    const std::wstring converted_path = strings::to_w_string(filepath);
    fail_check_(MFCreateSinkWriterFromURL(converted_path.c_str(), nullptr, nullptr, &sink_writer_), "Failed to create SinkWriter");

    // Ouput media type
    ComPtr<IMFMediaType> media_type_out = nullptr;
    fail_check_(MFCreateMediaType(&media_type_out), "Failed to create output media type");

    // Output media type info
    fail_check_(media_type_out->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video), "Failed to set major type [output media]");
    fail_check_(media_type_out->SetGUID(MF_MT_SUBTYPE, encoding_format_), "Failed to set subtype [output media]");
    fail_check_(media_type_out->SetUINT32(MF_MT_AVG_BITRATE, bit_rate_), "Failed to set average bitrate [output media]");
    fail_check_(media_type_out->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive), "Failed to set interlace mode [output media]");
    fail_check_(MFSetAttributeSize(media_type_out.Get(), MF_MT_FRAME_SIZE, width_, height_), "Failed to set frame size [output media]");
    fail_check_(MFSetAttributeRatio(media_type_out.Get(), MF_MT_FRAME_RATE, fps_, 1), "Failed to set FPS [output media]");
    fail_check_(MFSetAttributeRatio(media_type_out.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1), "Failed to set pixel aspect ratio [output media]");
    fail_check_(sink_writer_->AddStream(media_type_out.Get(), &stream_index_), "Failed to add output media type stream");

    // Input media type
    ComPtr<IMFMediaType> media_type_in = nullptr;
    fail_check_(MFCreateMediaType(&media_type_in), "Failed to create input media type");

    // Input media type info
    fail_check_(media_type_in->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video), "Failed to set major type [input media]");
    fail_check_(media_type_in->SetGUID(MF_MT_SUBTYPE, input_format_), "Failed to set subtype [input media]");
    fail_check_(media_type_in->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive), "Failed to set interlace mode [input media]");
    fail_check_(MFSetAttributeSize(media_type_in.Get(), MF_MT_FRAME_SIZE, width_, height_), "Failed to set frame size [input media]");
    fail_check_(MFSetAttributeRatio(media_type_in.Get(), MF_MT_FRAME_RATE, fps_, 1), "Failed to set FPS [input media]");
    fail_check_(MFSetAttributeRatio(media_type_in.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1), "Failed to set pixel aspect ratio [input media]");
    fail_check_(sink_writer_->SetInputMediaType(stream_index_, media_type_in.Get(), NULL), "Failed to add input media type stream");

    // Setup buffers
    fail_check_(MFCreateMemoryBuffer(frame_byte_size_, &media_buffer_), "Failed to create media buffer");
    fail_check_(media_buffer_->SetCurrentLength(frame_byte_size_), "Failed to set media buffer length");

    fail_check_(MFCreateSample(&media_sample_), "Failed to create media sample");
    fail_check_(media_sample_->AddBuffer(media_buffer_.Get()), "Failed to add media buffer to media sample");
    fail_check_(media_sample_->SetSampleDuration(frame_duration_), "Failed to set sample frame duration");

    // Start accepting data
    fail_check_(sink_writer_->BeginWriting(), "Failed to start accepting frames");
}

kl::video_writer::~video_writer()
{}

kl::int2 kl::video_writer::frame_size() const
{
    return { (int) width_, (int) height_ };
}

int kl::video_writer::fps() const
{
    return fps_;
}

int kl::video_writer::bit_rate() const
{
    return bit_rate_;
}

GUID kl::video_writer::format() const
{
    return encoding_format_;
}

bool kl::video_writer::add_frame(const image& frame)
{
    const int2 frame_size = this->frame_size();
    if (frame.width() != frame_size.x || frame.height() != frame_size.y) {
        return false;
    }

    const kl::color* in_buffer = frame;
    kl::color* out_buffer = nullptr;

    fail_check_(media_buffer_->Lock((BYTE**) &out_buffer, nullptr, nullptr), "Failed to lock the bytes [video_writer]");

    for (uint32_t y = 0; y < height_; y++) {
        memcpy(out_buffer + (height_ - 1 - y) * width_, in_buffer + y * width_, frame_byte_width_);
    }

    fail_check_(media_buffer_->Unlock(), "Failed to unlock bytes [video_writer]");

    if (!succeeded_(media_sample_->SetSampleTime(duration_100ns()))) {
        return false;
    }

    if (!succeeded_(sink_writer_->WriteSample(stream_index_, media_sample_.Get()))) {
        return false;
    }

    frame_counter_ += 1;
    return true;
}

int kl::video_writer::frame_count() const
{
    return frame_counter_;
}

uint64_t kl::video_writer::duration_100ns() const
{
    return frame_counter_ * frame_duration_;
}

float kl::video_writer::duration_seconds() const
{
    static constexpr float diver = 1.0f / 1e7f;
    return duration_100ns() * diver;
}

bool kl::video_writer::finalize()
{
    return succeeded_(sink_writer_->Finalize());
}
