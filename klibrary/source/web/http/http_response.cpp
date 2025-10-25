#include "klibrary.h"


void kl::HttpResponse::load_text( std::string_view const& text )
{
    content_type = HttpContentType::TEXT;
    content = text;
}

void kl::HttpResponse::load_html( Html const& html )
{
    content_type = HttpContentType::HTML;
    content = html.data;
}

void kl::HttpResponse::load_raw( void const* ptr, uint64_t byte_size )
{
    content_type = HttpContentType::OCTET_STREAM;
    content.resize( byte_size );
    std::memcpy( content.data(), ptr, byte_size );
}

void kl::HttpResponse::load_file( fs::path const& path, std::optional<ByteRange> const& byte_range )
{
    const auto filesize = fs::file_size( path );
    if ( filesize <= 0 )
        return;

    if ( auto opt_type = probe_content_type( path ) )
        content_type = *opt_type;
    else
        content_type = HttpContentType::OCTET_STREAM;

    if ( byte_range )
    {
        headers["Content-Range"] = format( "bytes ", byte_range->start_incl.value_or( 0 ), "-", byte_range->end_incl.value_or( filesize - 1 ), "/", filesize );
        status_code = HttpStatusCode::PARTIAL_CONTENT;
        content = read_file_string( path.generic_string(), *byte_range );
    }
    else
    {
        status_code = HttpStatusCode::OK;
        content = read_file_string( path.generic_string() );
    }
}

std::string kl::HttpResponse::compile() const
{
    std::stringstream response;
    response << http_version << " " << status_code << "\r\n";
    for ( const auto& [key, value] : headers )
        response << key << ": " << value << "\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "\r\n";

    std::string result = response.str();
    result.insert( result.end(), content.begin(), content.end() );
    return result;
}

std::optional<std::string> kl::probe_content_type( fs::path const& path )
{
    LPWSTR mime_out = nullptr;
    if ( FAILED( FindMimeFromData( NULL, path.filename().generic_wstring().data(), NULL, 0, NULL, FMFD_URLASFILENAME, &mime_out, 0x0 ) ) )
        return std::nullopt;
    std::wstring result( mime_out );
    CoTaskMemFree( mime_out );
    return convert_string( result );
}
