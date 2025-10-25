#include "klibrary.h"


kl::HttpQuery::HttpQuery( std::string_view const& full_path )
{
    const size_t index = full_path.find( '?' );
    if ( index == -1 )
    {
        path = full_path;
        return;
    }

    path = full_path.substr( 0, index );
    const std::string_view data = full_path.substr( index + 1 );
    for ( auto& part : kl::split_string( data, "&" ) )
    {
        const size_t index = part.find( '=' );
        const std::string key = decode_url_string( part.substr( 0, index ) );
        ( *this )[key] = ( index != -1 ) ? decode_url_string( part.substr( index + 1 ) ) : "";
    }
}

std::string kl::encode_url_string( std::string_view const& data )
{
    std::stringstream escaped;
    escaped.fill( '0' );
    escaped << std::hex;
    for ( byte c : data )
    {
        if ( std::isalnum( c ) || c == '-' || c == '_' || c == '.' || c == '~' )
        {
            escaped << c;
        }
        else if ( c == ' ' )
        {
            escaped << '+';
        }
        else
        {
            escaped << '%' << std::setw( 2 ) << int( c );
        }
    }
    return escaped.str();
}

std::string kl::decode_url_string( std::string_view const& data )
{
    std::string result;
    for ( size_t i = 0; i < data.size(); i++ )
    {
        if ( data[i] == '%' )
        {
            if ( i + 2 < data.size() )
            {
                const std::string substr{ data.data() + ( i + 1 ), 2 };
                const int parsed_int = std::stoi( substr, nullptr, 16 );
                result.push_back( parsed_int );
            }
            i += 2;
        }
        else if ( data[i] == '+' )
        {
            result.push_back( ' ' );
        }
        else
        {
            result.push_back( data[i] );
        }
    }
    return result;
}
