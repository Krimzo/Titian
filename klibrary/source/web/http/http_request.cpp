#include "klibrary.h"


std::optional<kl::ByteRange> kl::HttpRequest::byte_range() const
{
    const auto it = values.find( "Range" );
    if ( it == values.end() )
        return std::nullopt;

    const std::string_view strval = it->second;
    const std::vector parts = split_string( strval.substr( std::string_view{ "bytes=" }.size() ), "-" );
    if ( parts.size() != 2 )
        return std::nullopt;

    ByteRange range;
    range.start_incl = parse_int( parts[0] );
    range.end_incl = parse_int( parts[1] );
    return range;
}

std::optional<kl::HttpRequest> kl::HttpRequest::from_string( std::string_view const& data )
{
    std::vector lines = split_string( data, "\n" );
    if ( lines.empty() )
        return std::nullopt;

    std::vector first_parts = split_string( lines[0], " " );
    if ( first_parts.size() < 3 )
        return std::nullopt;

    HttpRequest request;
    request.request_type = first_parts[0];
    request.full_path = first_parts[1];
    request.http_version = first_parts[2];
    for ( size_t i = 1; i < lines.size(); i++ ) {
        auto const& line = lines[i];
        const size_t index = line.find( ": " );
        if ( index != -1 )
            request.values[line.substr( 0, index )] = line.substr( index + 2 );
    }
    return request;
}
