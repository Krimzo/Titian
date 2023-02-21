#pragma once

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

        ComPtr<IMFSinkWriter> sink_writer_ = nullptr;
        DWORD stream_index_ = 0;

        const int frame_byte_width_;
        const int frame_byte_size_;

        ComPtr<IMFMediaBuffer> media_buffer_ = nullptr;
        ComPtr<IMFSample> media_sample_ = nullptr;

        int frame_counter_ = 0;

    public:
        video_writer(const std::string& filepath, const kl::int2& frame_size, int fps, int bit_rate, const GUID& encoding_format);
        virtual ~video_writer();

        video_writer(const video_writer&) = delete;
        video_writer(const video_writer&&) = delete;

        void operator=(const video_writer&) = delete;
        void operator=(const video_writer&&) = delete;

        int2 frame_size() const;
        int fps() const;

        int bit_rate() const;
        GUID format() const;

        bool add_frame(const image& frame);
        int frame_count() const;

        uint64_t duration_100ns() const;
        float duration_seconds() const;

        bool finalize();
    };
}
