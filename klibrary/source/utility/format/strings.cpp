#include "klibrary.h"


std::wstring kl::convert_string( std::string_view const& source )
{
    std::wstring result;
    result.resize( MultiByteToWideChar( CP_UTF8, 0, source.data(), (int) source.size(), nullptr, 0 ) );
    MultiByteToWideChar( CP_UTF8, 0, source.data(), (int) source.size(), result.data(), (int) result.size() );
    return result;
}

std::string kl::convert_string( std::wstring_view const& source )
{
    std::string result;
    result.resize( WideCharToMultiByte( CP_UTF8, 0, source.data(), (int) source.size(), nullptr, 0, nullptr, nullptr ) );
    WideCharToMultiByte( CP_UTF8, 0, source.data(), (int) source.size(), result.data(), (int) result.size(), nullptr, nullptr );
    return result;
}

std::vector<std::string> kl::split_string( std::string_view const& data, std::string_view const& delimiter )
{
    std::vector<std::string> parts;
    for ( auto const& part : std::views::split( data, delimiter ) )
        parts.emplace_back( part.begin(), part.end() );
    return parts;
}

std::vector<std::wstring> kl::split_string( std::wstring_view const& data, std::wstring_view const& delimiter )
{
    std::vector<std::wstring> parts;
    for ( auto const& part : std::views::split( data, delimiter ) )
        parts.emplace_back( part.begin(), part.end() );
    return parts;
}

void kl::replace_all( std::string& str, std::string_view const& from, std::string_view const& to )
{
    if ( str.empty() || from.empty() )
        return;

    size_t index = 0;
    while ( ( index = str.find( from, index ) ) != -1 )
    {
        str.replace( index, from.length(), to );
        index += to.length();
    }
}

void kl::replace_all( std::wstring& str, std::wstring_view const& from, std::wstring_view const& to )
{
    if ( str.empty() || from.empty() )
        return;

    size_t index = 0;
    while ( ( index = str.find( from, index ) ) != -1 )
    {
        str.replace( index, from.length(), to );
        index += to.length();
    }
}

std::optional<int64_t> kl::parse_int( std::string_view const& data )
{
    if ( data.empty() )
        return std::nullopt;

    char* last_char = nullptr;
    int64_t result = std::strtoll( data.data(), &last_char, 10 );
    if ( last_char != data.data() + data.size() )
        return std::nullopt;

    return { result };
}

std::optional<double> kl::parse_float( std::string_view const& data )
{
    if ( data.empty() )
        return std::nullopt;

    char* last_char = nullptr;
    double result = std::strtod( data.data(), &last_char );
    if ( last_char != data.data() + data.size() )
        return std::nullopt;

    return { result };
}
