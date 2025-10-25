#include "klibrary.h"


kl::File::File( std::string_view const& filepath, bool write )
{
    open( filepath, write );
}

kl::File::File( std::wstring_view const& filepath, bool write )
{
    open( filepath, write );
}

kl::File::~File()
{
    close();
}

kl::File::operator bool() const
{
    return (bool) m_file;
}

void kl::File::open( std::string_view const& filepath, bool write )
{
    close();
    fopen_s( &m_file, filepath.data(), write ? "wb" : "rb" );
}

void kl::File::open( std::wstring_view const& filepath, bool write )
{
    close();
    _wfopen_s( &m_file, filepath.data(), write ? L"wb" : L"rb" );
}

void kl::File::close()
{
    if ( !m_file )
        return;
    fclose( m_file );
    m_file = nullptr;
}

bool kl::File::seek( int64_t position ) const
{
    if ( !m_file )
        return false;
    if ( position < 0 )
        return !fseek( m_file, (long) position + 1, SEEK_END );
    return !fseek( m_file, (long) position, SEEK_SET );
}

bool kl::File::move( int64_t delta ) const
{
    if ( !m_file )
        return false;
    return !fseek( m_file, (long) delta, SEEK_CUR );
}

bool kl::File::rewind() const
{
    return seek( 0 );
}

bool kl::File::unwind() const
{
    return seek( -1 );
}

uint64_t kl::File::tell() const
{
    if ( !m_file )
        return -1;
    return ftell( m_file );
}

std::string kl::file_extension( std::string_view const& filepath )
{
    return fs::path( filepath ).extension().string();
}

std::wstring kl::wfile_extension( std::wstring_view const& filepath )
{
    return fs::path( filepath ).extension().wstring();
}

std::vector<std::string> kl::list_files( std::string_view const& path, bool recursive )
{
    std::vector<std::string> files;
    if ( recursive )
    {
        for ( auto const& entry : fs::recursive_directory_iterator( path ) )
        {
            if ( !entry.is_directory() )
                files.emplace_back( fs::absolute( entry ).generic_string() );
        }
    }
    else
    {
        for ( auto const& entry : fs::directory_iterator( path ) )
        {
            if ( !entry.is_directory() )
                files.emplace_back( fs::absolute( entry ).generic_string() );
        }
    }
    return files;
}

std::vector<std::wstring> kl::list_files( std::wstring_view const& path, bool recursive )
{
    std::vector<std::wstring> files;
    if ( recursive )
    {
        for ( auto const& entry : fs::recursive_directory_iterator( path ) )
        {
            if ( !entry.is_directory() )
                files.emplace_back( fs::absolute( entry ).generic_wstring() );
        }
    }
    else
    {
        for ( auto const& entry : fs::directory_iterator( path ) )
        {
            if ( !entry.is_directory() )
                files.emplace_back( fs::absolute( entry ).generic_wstring() );
        }
    }
    return files;
}

std::string kl::read_file_string( std::string_view const& filepath, ByteRange const& byte_range )
{
    File file{ filepath, false };
    if ( !file )
        return {};

    file.unwind();
    const int64_t file_size = (int64_t) file.tell();
    if ( file_size <= 0 )
        return {};

    const int64_t first_byte = clamp( byte_range.start_incl.value_or( 0 ), 0ll, file_size - 1 );
    const int64_t last_byte = clamp( byte_range.end_incl.value_or( file_size - 1 ), 0ll, file_size - 1 );

    std::string result;
    result.resize( max( last_byte - first_byte + 1, 0ll ) );
    file.seek( first_byte );
    file.read( result.data(), result.size() );
    return result;
}

std::string kl::read_file_string( std::wstring_view const& filepath, ByteRange const& byte_range )
{
    File file{ filepath, false };
    if ( !file )
        return {};

    file.unwind();
    const int64_t file_size = (int64_t) file.tell();
    if ( file_size <= 0 )
        return {};

    const int64_t first_byte = clamp( byte_range.start_incl.value_or( 0 ), 0ll, file_size - 1 );
    const int64_t last_byte = clamp( byte_range.end_incl.value_or( file_size - 1 ), 0ll, file_size - 1 );

    std::string result;
    result.resize( max( last_byte - first_byte + 1, 0ll ) );
    file.seek( first_byte );
    file.read( result.data(), result.size() );
    return result;
}

bool kl::write_file_string( std::string_view const& filepath, std::string_view const& data )
{
    File file{ filepath, true };
    if ( !file )
        return false;

    file.write<char>( data.data(), data.size() );
    return true;
}

bool kl::write_file_string( std::wstring_view const& filepath, std::string_view const& data )
{
    File file{ filepath, true };
    if ( !file )
        return false;

    file.write<char>( data.data(), data.size() );
    return true;
}

std::optional<std::string> kl::choose_file( bool save, std::vector<std::pair<std::string_view, std::string_view>> const& filters, int* out_index )
{
    std::stringstream filter_buffer;
    for ( auto const& filter : filters )
        filter_buffer << filter.first << " (*" << filter.second << ")" << '\0' << '*' << filter.second << '\0';
    const std::string filter_data = filter_buffer.str();

    char file_buffer[512] = {};
    OPENFILENAMEA dialog_info = {};
    dialog_info.lStructSize = sizeof( dialog_info );
    dialog_info.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    dialog_info.lpstrFile = file_buffer;
    dialog_info.nMaxFile = sizeof( file_buffer ) - 1;
    dialog_info.lpstrFilter = filter_data.data();
    dialog_info.nFilterIndex = 1;

    std::optional<std::string> result;
    if ( ( save ? GetSaveFileNameA : GetOpenFileNameA )( &dialog_info ) )
    {
        result = std::string{ file_buffer };
        if ( out_index )
            *out_index = (int) dialog_info.nFilterIndex - 1;
    }
    SetCursor( LoadCursor( NULL, IDC_ARROW ) );
    return result;
}

std::optional<std::wstring> kl::wchoose_file( bool save, std::vector<std::pair<std::wstring_view, std::wstring_view>> const& filters, int* out_index )
{
    std::wstringstream filter_buffer;
    for ( auto const& filter : filters )
        filter_buffer << filter.first << " (*" << filter.second << ")" << '\0' << '*' << filter.second << '\0';
    const std::wstring filter_data = filter_buffer.str();

    wchar_t file_buffer[512] = {};
    OPENFILENAMEW dialog_info = {};
    dialog_info.lStructSize = sizeof( dialog_info );
    dialog_info.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    dialog_info.lpstrFile = file_buffer;
    dialog_info.nMaxFile = sizeof( file_buffer ) - 1;
    dialog_info.lpstrFilter = filter_data.data();
    dialog_info.nFilterIndex = 1;

    std::optional<std::wstring> result;
    if ( ( save ? GetSaveFileNameW : GetOpenFileNameW )( &dialog_info ) )
    {
        result = std::wstring{ file_buffer };
        if ( out_index )
            *out_index = (int) dialog_info.nFilterIndex - 1;
    }
    SetCursor( LoadCursor( NULL, IDC_ARROW ) );
    return result;
}

std::vector<kl::Vertex> kl::parse_obj_file( fs::path const& filepath, bool flip_z )
{
    std::ifstream file{ filepath };
    if ( !verify( file.is_open(), "Failed to open file ", filepath ) )
        return {};

    std::vector<Float3> position_data;
    std::vector<Float3> normal_data;
    std::vector<Float2> uv_data;
    std::vector<Vertex> vertex_data;

    float z_flip = flip_z ? -1.0f : 1.0f;
    for ( std::string line; std::getline( file, line );)
    {
        const std::vector<std::string> parts = split_string( line, " " );

        if ( parts.size() == 4 && parts.front() == "v" )
        {
            Float3 position{};
            position.x = (float) parse_float( parts[1] ).value_or( 0.0 );
            position.y = (float) parse_float( parts[2] ).value_or( 0.0 );
            position.z = (float) parse_float( parts[3] ).value_or( 0.0 ) * z_flip;
            position_data.push_back( position );
            continue;
        }

        if ( parts.size() == 4 && parts.front() == "vn" )
        {
            Float3 normal{};
            normal.x = (float) parse_float( parts[1] ).value_or( 0.0 );
            normal.y = (float) parse_float( parts[2] ).value_or( 0.0 );
            normal.z = (float) parse_float( parts[3] ).value_or( 0.0 ) * z_flip;
            normal_data.push_back( normal );
            continue;
        }

        if ( parts.size() == 3 && parts.front() == "vt" )
        {
            Float2 uv{};
            uv.x = (float) parse_float( parts[1] ).value_or( 0.0 );
            uv.y = (float) parse_float( parts[2] ).value_or( 0.0 );
            uv_data.push_back( uv );
            continue;
        }

        if ( parts.size() == 4 && parts.front() == "f" )
        {
            for ( int i = 1; i <= 3; i++ )
            {
                std::vector<std::string> line_part_parts = split_string( parts[i], "/" );
                if ( line_part_parts.size() != 3 )
                    continue;

                Vertex vertex;
                if ( uint64_t index = parse_int( line_part_parts[0] ).value_or( 0 ) - 1; index >= 0 && index < position_data.size() )
                    vertex.position = position_data[index];
                if ( uint64_t index = parse_int( line_part_parts[2] ).value_or( 0 ) - 1; index >= 0 && index < normal_data.size() )
                    vertex.normal = normal_data[index];
                if ( uint64_t index = parse_int( line_part_parts[1] ).value_or( 0 ) - 1; index >= 0 && index < uv_data.size() )
                    vertex.uv = uv_data[index];
                vertex_data.push_back( vertex );
            }
        }
    }
    return vertex_data;
}
