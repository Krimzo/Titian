#pragma once

#include "math/math.h"


// Helper
namespace kl {
    std::string get_file_extension(const std::string& filepath);
    std::vector<std::string> get_files(const std::string& path, bool recursive = false);

    std::string read_file_string(const std::string& filepath);
    bool write_file_string(const std::string& filepath, const std::string& data);
    bool append_file_string(const std::string& filepath, const std::string& data, int position = -1);

    std::vector<vertex> parse_obj_file(const std::string& filepath, bool flip_z = true);
}

// File
namespace kl {
    class file
    {
        FILE* file_ = nullptr;

    public:
        file();
        file(const std::string& filepath, bool clear = true);
        ~file();

        file(const file&) = delete;
        file(const file&&) = delete;

        void operator=(const file&) = delete;
        void operator=(const file&&) = delete;

        operator bool() const;

        bool open(const std::string& filepath, bool clear = true);
        void close();

        bool seek(int position) const;
        bool move(int delta) const;

        bool rewind() const;
        bool unwind() const;

        int tell() const;

        template <typename T>
        size_t read(T& object) const
        {
            return read(&object, 1);
        }

        template <typename T>
        size_t read(T* buffer, const size_t count) const
        {
            if (file_) {
                return fread(buffer, sizeof(T), count, file_);
            }
            return 0;
        }

        template <typename T>
        size_t write(const T& object)
        {
            return write(&object, 1);
        }

        template <typename T>
        size_t write(const T* buffer, const size_t count)
        {
            if (file_) {
                return fwrite(buffer, sizeof(T), count, file_);
            }
            return 0;
        }
    };
}
