#pragma once

#include "media/image/image.h"


namespace kl {
    class VideoWriter
    {
        const uint32_t m_width;
        const uint32_t m_height;
        const uint32_t m_fps;
        const uint32_t m_bit_rate;
        const GUID     m_encoding_format;

        const uint32_t m_pixel_count;
        const uint64_t m_frame_duration;
        const GUID     m_input_format = MFVideoFormat_RGB32;

        ComPtr<IMFSinkWriter> m_sink_writer = nullptr;
        DWORD m_stream_index = 0;

        const int m_frame_byte_width;
        const int m_frame_byte_size;

        ComPtr<IMFMediaBuffer> m_media_buffer = nullptr;
        ComPtr<IMFSample> m_media_sample = nullptr;

        int m_frame_counter = 0;

    public:
        VideoWriter(const std::string& filepath, const kl::Int2& frame_size, int fps, int bit_rate, const GUID& encoding_format);

        VideoWriter(const VideoWriter&) = delete;
        VideoWriter(const VideoWriter&&) = delete;

        void operator=(const VideoWriter&) = delete;
        void operator=(const VideoWriter&&) = delete;

        Int2 frame_size() const;
        int fps() const;

        int bit_rate() const;
        GUID format() const;

        bool add_frame(const Image& frame);
        int frame_count() const;

        uint64_t duration_100ns() const;
        float duration_seconds() const;

        bool finalize() const;
    };
}
