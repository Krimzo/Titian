#pragma once

#include "math/math.h"


namespace kl
{
struct File : NoCopy
{
    File();
    File( std::string_view const& filepath, bool write );
    ~File();

    operator bool() const;

    void open( std::string_view const& filepath, bool write );
    void close();

    bool seek( int64_t position ) const;
    bool move( int64_t delta ) const;

    bool rewind() const;
    bool unwind() const;

    int64_t tell() const;

    template<typename T>
    T read() const
    {
        T result = {};
        read<T>( result );
        return result;
    }

    template<typename T>
    uint64_t read( T& object ) const
    {
        return read( &object, 1 );
    }

    template<typename T>
    uint64_t read( T* buffer, uint64_t count ) const
    {
        if ( !m_file )
            return 0;
        return fread( buffer, sizeof( T ), count, m_file );
    }

    template<typename T>
    uint64_t write( T const& object )
    {
        return write( &object, 1 );
    }

    template<typename T>
    uint64_t write( T  const* buffer, uint64_t count )
    {
        if ( !m_file )
            return 0;
        return fwrite( buffer, sizeof( T ), count, m_file );
    }

private:
    FILE* m_file = nullptr;
};
}

namespace kl
{
std::string file_extension( std::string_view const& filepath );
std::vector<std::string> list_files( std::string_view const& path, bool recursive = false );

std::string read_file( std::string_view const& filepath );
bool write_file( std::string_view const& filepath, std::string_view const& data );

std::vector<Vertex> parse_obj_file( std::string_view const& filepath, bool flip_z = true );
std::optional<std::string> choose_file( bool save, std::vector<std::pair<std::string_view, std::string_view>> const& filters = { { "All Files", ".*" } }, int* out_index = nullptr );
}
