#pragma once

#include "math/math.h"


// Helper
namespace kl {
    std::string file_extension(const std::string& filepath);
    std::vector<std::string> list_files(const std::string& path, bool recursive = false);

    std::string read_file_string(const std::string& filepath);
    bool write_file_string(const std::string& filepath, const std::string& data);
    bool append_file_string(const std::string& filepath, const std::string& data, int position = -1);

    std::vector<Vertex> parse_obj_file(const std::string& filepath, bool flip_z = true);
    std::optional<std::string> choose_file(bool save, const std::vector<std::string>& filters = { "All" });
}

// File
namespace kl {
    class File
    {
        FILE* m_file = nullptr;

    public:
        File();
        File(const std::string& filepath, bool clear);
        virtual ~File();

        File(const File&) = delete;
        File(const File&&) = delete;

        void operator=(const File&) = delete;
        void operator=(const File&&) = delete;

        operator bool() const;

        bool open(const std::string& filepath, bool clear);
        void close();

        bool seek(int position) const;
        bool move(int delta) const;

        bool rewind() const;
        bool unwind() const;

        int tell() const;

        // Read
        template<typename T>
        T read() const
        {
            T result = {};
            read<T>(result);
            return result;
        }

        template <typename T>
        size_t read(T& object) const
        {
            return read(&object, 1);
        }

        template <typename T>
        size_t read(T* buffer, const size_t count) const
        {
            if (m_file) {
                return fread(buffer, sizeof(T), count, m_file);
            }
            return 0;
        }

        // Write
        template <typename T>
        size_t write(const T& object)
        {
            return write(&object, 1);
        }

        template <typename T>
        size_t write(const T* buffer, const size_t count)
        {
            if (m_file) {
                return fwrite(buffer, sizeof(T), count, m_file);
            }
            return 0;
        }
    };
}
