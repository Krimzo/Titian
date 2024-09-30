#pragma once

#include "media/image/image.h"


namespace kl {
    struct VideoType
    {
        static VideoType h264(eAVEncH264VProfile profile = eAVEncH264VProfile::eAVEncH264VProfile_Main)
        {
            return { MFVideoFormat_H264, (int) profile };
        }

        GUID type() const { return m_type; }
        int profile() const { return m_profile; }

    private:
        VideoType(const GUID type, const int profile)
            : m_type(type), m_profile(profile)
        {}

        GUID m_type = {};
        int m_profile = 0;
    };
}

namespace kl {
    struct VideoWriter : NoCopy
    {
        VideoWriter(const std::string_view& filepath, const VideoType& video_type, Int2 frame_size, int fps, int video_bit_rate, int audio_sample_rate);

        Int2 frame_size() const;
        int fps() const;
        int video_bit_rate() const;
        int frame_count() const;
        bool add_frame(const Image& frame);
        uint64_t video_duration_100ns() const;
        float video_duration_seconds() const;

        int audio_sample_rate() const;
        bool add_audio(const Audio& audio);
        uint64_t audio_duration_100ns() const;
        float audio_duration_seconds() const;

        void finalize() const;

    private:
        const uint32_t m_width;
        const uint32_t m_height;
        const uint32_t m_fps;
        const uint32_t m_bit_rate;
        const uint32_t m_sample_rate;
        const uint64_t m_frame_duration;

        ComRef<IMFSinkWriter> m_writer;
        DWORD m_video_index = 0;
        DWORD m_audio_index = 0;
        uint64_t m_video_time = 0;
        uint64_t m_audio_time = 0;
    };
}
