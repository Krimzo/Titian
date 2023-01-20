#pragma once

#include "window/system_handler.h"
#include "media/image.h"


namespace kl {
    class video
    {
        IMFSourceReader* reader_ = nullptr;

    public:
        video();
        explicit video(const std::string& filepath);
        ~video();

        video(const video&) = delete;
        video(const video&&) = delete;
        
        void operator=(const video&) = delete;
        void operator=(const video&&) = delete;
        
        [[nodiscard]] bool is_open() const;

        void open(const std::string& filepath);
        
        void close();

        [[nodiscard]] size_t size() const;

        [[nodiscard]] float duration() const;

        [[nodiscard]] float fps() const;

        [[nodiscard]] int frame_count() const;

        [[nodiscard]] int2 frame_size() const;

        [[nodiscard]] bool get_next_frame(image& out) const;
    };
}
