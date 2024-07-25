#pragma once

#include "media/image/image.h"


namespace kl {
    class VideoWriter
    {
        const GUID m_output_format;
        const GUID m_input_format = MFVideoFormat_RGB32;

        const uint32_t m_width;
        const uint32_t m_height;
        const uint32_t m_fps;
        const uint32_t m_bit_rate;
        const uint32_t m_sample_rate;
        const uint64_t m_frame_duration;

        ComRef<IMFSinkWriter> m_writer = {};
        DWORD m_video_index = 0;
        DWORD m_audio_index = 0;
        uint64_t m_video_time = 0;
        uint64_t m_audio_time = 0;

    public:
        VideoWriter(const std::string& filepath, const GUID& output_format, const Int2& frame_size, int fps, int video_bit_rate, int audio_sample_rate);

        VideoWriter(const VideoWriter&) = delete;
        VideoWriter(VideoWriter&&) = delete;

        void operator=(const VideoWriter&) = delete;
        void operator=(VideoWriter&&) = delete;

        GUID output_format() const;
        Int2 frame_size() const;
        int fps() const;
        int frame_count() const;

        int video_bit_rate() const;
        bool add_frame(const Image& frame);
        uint64_t video_duration_100ns() const;
        float video_duration_seconds() const;

        int audio_sample_rate() const;
        bool add_audio(const Audio& audio);
        uint64_t audio_duration_100ns() const;
        float audio_duration_seconds() const;

        void finalize() const;
    };
}
