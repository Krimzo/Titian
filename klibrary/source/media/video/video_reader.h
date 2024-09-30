#pragma once

#include "media/image/image.h"
#include "graphics/gpu/gpu.h"
#include "memory/safety/ref.h"


namespace kl {
    struct VideoReader : NoCopy
    {
        VideoReader(const std::string_view& filepath, Int2 output_size = {}, bool use_gpu = true);

        uint64_t byte_size() const;

        int64_t duration_100ns() const;
        float duration_seconds() const;

        Int2 frame_size() const;
        int frame_count() const;
        float fps() const;

        bool seek(float time) const;
        bool read_frame(Image& out, int* out_index = nullptr) const;

    private:
        uint64_t m_byte_size = 0;
        int64_t m_duration = 0;

        Int2 m_frame_size = {};
        int m_frame_byte_size = 0;

        int m_frame_count = 0;
        float m_fps = 0.0f;

        ComRef<IMFSourceReader> m_reader;
        Ref<VideoGPU> m_gpu;
    };
}
