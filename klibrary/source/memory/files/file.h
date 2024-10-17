#pragma once

#include "math/math.h"


namespace kl {
    struct File : NoCopy
    {
        File();
        File(const std::string_view& filepath, bool write);
        ~File();

        operator bool() const;

        void open(const std::string_view& filepath, bool write);
        void close();

        bool seek(int64_t position) const;
        bool move(int64_t delta) const;

        bool rewind() const;
        bool unwind() const;

        int64_t tell() const;

        template<typename T>
        T read() const
        {
            T result = {};
            read<T>(result);
            return result;
        }

        template<typename T>
        uint64_t read(T& object) const
        {
            return read(&object, 1);
        }

        template<typename T>
        uint64_t read(T* buffer, const uint64_t count) const
        {
            if (m_file) {
                return fread(buffer, sizeof(T), count, m_file);
            }
            return 0;
        }

        template<typename T>
        uint64_t write(const T& object)
        {
            return write(&object, 1);
        }

        template<typename T>
        uint64_t write(const T* buffer, const uint64_t count)
        {
            if (m_file) {
                return fwrite(buffer, sizeof(T), count, m_file);
            }
            return 0;
        }

    private:
        FILE* m_file = nullptr;
    };
}

namespace kl {
    std::string file_extension(const std::string_view& filepath);
    std::vector<std::string> list_files(const std::string_view& path, bool recursive = false);

    std::string read_file(const std::string_view& filepath);
    bool write_file(const std::string_view& filepath, const std::string_view& data);

    std::vector<Vertex> parse_obj_file(const std::string_view& filepath, bool flip_z = true);
    std::optional<std::string> choose_file(bool save, const std::vector<std::pair<std::string_view, std::string_view>>& filters = { { "All Files", ".*" } }, int* out_index = nullptr);
}
