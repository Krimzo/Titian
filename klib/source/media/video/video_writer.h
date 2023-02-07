#pragma once

#include "window/system_handler.h"
#include "media/image/image.h"


namespace kl {
    class video_writer
    {
        const uint32_t width_;
        const uint32_t height_;
        const uint32_t fps_;
        const uint32_t bit_rate_;
        const GUID     encoding_format_;

        const uint32_t pixel_count_;
        const uint64_t frame_duration_;
        const GUID     input_format_ = MFVideoFormat_RGB32;

        IMFSinkWriter* sink_writer_ = nullptr;
        DWORD stream_index_ = 0;

        const int frame_byte_width_;
        const int frame_byte_size_;

        IMFMediaBuffer* media_buffer_ = nullptr;
        IMFSample* media_sample_ = nullptr;

        int frame_counter_ = 0;

    public:
        video_writer(const std::string& filepath, const kl::int2& frame_size, int fps, int bit_rate, const GUID& encoding_format);
        ~video_writer();

        video_writer(const video_writer&) = delete;
        video_writer(const video_writer&&) = delete;

        void operator=(const video_writer&) = delete;
        void operator=(const video_writer&&) = delete;

        [[nodiscard]] int2 frame_size() const;

        [[nodiscard]] int fps() const;

        [[nodiscard]] int bit_rate() const;

        [[nodiscard]] GUID format() const;

        bool add_frame(const image& frame);

        [[nodiscard]] int frame_count() const;

        [[nodiscard]] uint64_t duration_100ns() const;

        [[nodiscard]] float duration_seconds() const;

        bool finalize();
    };
}
