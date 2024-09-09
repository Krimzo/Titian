#pragma once

#include "apis/apis.h"


namespace kl {
    using AudioStorage = std::vector<float>;
}

namespace kl {
    enum struct AudioType : int32_t
    {
        WAV = 0,
        MP3,
    };
}

namespace kl {
    struct Audio : AudioStorage
    {
        int sample_rate = 0;

        Audio();
        Audio(int sample_rate);
        Audio(const std::string_view& path);

        uint64_t duration_100ns() const;
        float duration_seconds() const;
        void set_duration(float duration);

        void increase_volume(float amount);
		void decrease_volume(float amount);

        float index_to_time(int at_index) const;
        int time_to_index(float at_time) const;

        float sample_at_index(int index) const;
        float sample_at_time(float time) const;

        bool load_from_memory(const void* data, uint64_t byte_size);
        bool load_from_buffer(const std::string_view& buffer);
        bool load_from_file(const std::string_view& filepath);

        bool save_to_buffer(std::string& buffer, AudioType type) const;
        bool save_to_file(const std::string_view& filepath, AudioType type) const;
    };
}
