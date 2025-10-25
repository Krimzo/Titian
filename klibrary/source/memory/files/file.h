#pragma once

#include "math/math.h"


namespace kl
{
struct ByteRange
{
    std::optional<int64_t> start_incl;
    std::optional<int64_t> end_incl;
};

struct File : NoCopy
{
    File() = default;
    File( std::string_view const& filepath, bool write = false );
    File( std::wstring_view const& filepath, bool write = false );
    ~File() noexcept;

    operator bool() const;

    void open( std::string_view const& filepath, bool write );
    void open( std::wstring_view const& filepath, bool write );
    void close();

    bool seek( int64_t position ) const;
    bool move( int64_t delta ) const;

    bool rewind() const;
    bool unwind() const;

    uint64_t tell() const;

    template<typename T>
    T read( uint64_t* opt_out ) const
    {
        T result{};
        if ( opt_out )
            *opt_out = read<T>( result );
        else
            read<T>( result );
        return result;
    }

    template<typename T>
    uint64_t read( T& object ) const
    {
        return read<T>( &object, 1 );
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
        return write<T>( &object, 1 );
    }

    template<typename T>
    uint64_t write( T const* buffer, uint64_t count )
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
std::wstring wfile_extension( std::wstring_view const& filepath );

std::vector<std::string> list_files( std::string_view const& path, bool recursive = false );
std::vector<std::wstring> list_files( std::wstring_view const& path, bool recursive = false );

std::string read_file_string( std::string_view const& filepath, ByteRange const& byte_range = {} );
std::string read_file_string( std::wstring_view const& filepath, ByteRange const& byte_range = {} );

bool write_file_string( std::string_view const& filepath, std::string_view const& data );
bool write_file_string( std::wstring_view const& filepath, std::string_view const& data );

std::optional<std::string> choose_file( bool save, std::vector<std::pair<std::string_view, std::string_view>> const& filters = { { "All Files", ".*" } }, int* out_index = nullptr );
std::optional<std::wstring> wchoose_file( bool save, std::vector<std::pair<std::wstring_view, std::wstring_view>> const& filters = { { L"All Files", L".*" } }, int* out_index = nullptr );

std::vector<Vertex> parse_obj_file( fs::path const& filepath, bool flip_z = true );
}
