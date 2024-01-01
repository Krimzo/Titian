#pragma once

#include "media/image/image.h"


namespace kl {
    class VideoReader
    {
        uint64_t m_byte_size = 0;
        int64_t m_duration = 0;

        Int2 m_frame_size = {};
        int m_frame_byte_size = 0;

        int m_frame_count = 0;
        float m_fps = 0.0f;

        ComPtr<IMFSourceReader> m_reader = nullptr;

    public:
        VideoReader(const std::string& filepath);

        VideoReader(const VideoReader&) = delete;
        VideoReader(const VideoReader&&) = delete;

        void operator=(const VideoReader&) = delete;
        void operator=(const VideoReader&&) = delete;

        uint64_t byte_size() const;

        int64_t duration_100ns() const;
        float duration_seconds() const;

        Int2 frame_size() const;
        int frame_count() const;

        float fps() const;
        bool next_frame(Image& out) const;
    };
}
