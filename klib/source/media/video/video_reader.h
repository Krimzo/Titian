#pragma once

#include "window/system_handler.h"
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

        IMFSourceReader* reader_ = nullptr;

    public:
        video_reader(const std::string& filepath);
        ~video_reader();

        video_reader(const video_reader&) = delete;
        video_reader(const video_reader&&) = delete;
        
        void operator=(const video_reader&) = delete;
        void operator=(const video_reader&&) = delete;

        [[nodiscard]] uint64_t byte_size() const;

        [[nodiscard]] int64_t duration_100ns() const;

        [[nodiscard]] float duration_seconds() const;
        
        [[nodiscard]] int2 frame_size() const;

        [[nodiscard]] int frame_count() const;

        [[nodiscard]] float fps() const;

        [[nodiscard]] bool get_next_frame(image& out) const;
    };
}
