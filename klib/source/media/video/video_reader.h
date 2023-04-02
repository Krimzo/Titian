#pragma once

#include "media/image/image.h"


namespace kl {
    class video_reader
    {
        uint64_t byte_size_ = 0;
        int64_t duration_ = 0;

        int2 frame_size_ = {};
        int frame_byte_size_ = 0;

        int frame_count_ = 0;
        float fps_ = 0.0f;

        ComPtr<IMFSourceReader> reader_ = nullptr;

    public:
        video_reader(const std::string& filepath);
        ~video_reader();

        video_reader(const video_reader&) = delete;
        video_reader(const video_reader&&) = delete;

        void operator=(const video_reader&) = delete;
        void operator=(const video_reader&&) = delete;

        uint64_t byte_size() const;

        int64_t duration_100ns() const;
        float duration_seconds() const;

        int2 frame_size() const;
        int frame_count() const;

        float fps() const;
        bool get_next_frame(image& out) const;
    };
}
