#include "klibrary.h"


std::string kl::download_website( std::string_view const& url, int buffer_size )
{
    Internet connection = InternetOpenA( "Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0 );
    if ( !verify( connection, "Failed to open a browser" ) )
        return {};

    Internet address = InternetOpenUrlA( connection, url.data(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0 );
    if ( !verify( address, "Failed to open url \"", url, "\"" ) )
        return {};

    std::string data;
    std::vector<byte> buffer( buffer_size );
    for ( DWORD bytes_read = 0; InternetReadFile( address, buffer.data(), (DWORD) buffer.size(), &bytes_read ) && bytes_read > 0;)
        data.insert( data.end(), buffer.begin(), buffer.begin() + bytes_read );
    return data;
}

int kl::send_http_request( std::string_view const& url, int port, std::string_view const& object, std::string_view const& verb, char const* headers, DWORD headers_size, void const* optional, DWORD optional_size )
{
    Internet browser = InternetOpenA( "Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0 );
    if ( !browser )
        return 1;

    Internet internet = InternetConnectA( browser, url.data(), port, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0 );
    if ( !internet )
        return 2;

    Internet request = HttpOpenRequestA( internet, verb.data(), object.data(), nullptr, nullptr, nullptr, INTERNET_FLAG_SECURE, 0 );
    if ( !request )
        return 3;

    if ( !HttpSendRequestA( request, headers, headers_size, (void*) optional, optional_size ) )
        return 4;
    return 0;
}
