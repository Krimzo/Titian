#pragma once

#include "apis/apis.h"


namespace kl {
    using AudioStorage = std::vector<float>;
}

namespace kl {
    enum class AudioType
    {
        WAV = 0,
        MP3,
    };
}

namespace kl {
    class Audio : public AudioStorage
    {
    public:
        int sample_rate = 0;

        Audio();
        Audio(int sample_rate);
        Audio(const std::string_view& path);

        // Alter
        uint64_t duration_100ns() const;
        float duration_seconds() const;
        void set_duration(float duration);

        void increase_volume(float amount);
		void decrease_volume(float amount);

        // Helper
        float index_to_time(int at_index) const;
        int time_to_index(float at_time) const;

        // Read
        float sample_at_index(int index) const;
        float sample_at_time(float time) const;

        // Decoding
        bool load_from_memory(const byte* data, uint64_t byte_size);
        bool load_from_vector(const std::vector<byte>& buffer);
        bool load_from_file(const std::string_view& filepath);

        // Encoding
        bool save_to_vector(std::vector<byte>* buffer, AudioType type) const;
        bool save_to_file(const std::string_view& filepath, AudioType type) const;
    };
}
