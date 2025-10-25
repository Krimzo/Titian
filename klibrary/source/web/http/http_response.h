#pragma once

#include "web/http/http_query.h"
#include "web/html/html.h"


namespace kl
{
struct HttpVersion
{
    static constexpr std::string_view _10 = "HTTP/1.0";
    static constexpr std::string_view _11 = "HTTP/1.1";
    static constexpr std::string_view _20 = "HTTP/2.0";
    static constexpr std::string_view _30 = "HTTP/3.0";
};

struct HttpStatusCode
{
    static constexpr std::string_view OK = "200 OK";
    static constexpr std::string_view CREATED = "201 Created";
    static constexpr std::string_view ACCEPTED = "202 Accepted";
    static constexpr std::string_view NO_CONTENT = "204 No Content";
    static constexpr std::string_view PARTIAL_CONTENT = "206 Partial Content";

    static constexpr std::string_view MOVED_PERMANENTLY = "301 Moved Permanently";
    static constexpr std::string_view FOUND = "302 Found";
    static constexpr std::string_view SEE_OTHER = "303 See Other";
    static constexpr std::string_view NOT_MODIFIED = "304 Not Modified";
    static constexpr std::string_view TEMPORARY_REDIRECT = "307 Temporary Redirect";
    static constexpr std::string_view PERMANENT_REDIRECT = "308 Permanent Redirect";

    static constexpr std::string_view BAD_REQUEST = "400 Bad Request";
    static constexpr std::string_view UNAUTHORIZED = "401 Unauthorized";
    static constexpr std::string_view FORBIDDEN = "403 Forbidden";
    static constexpr std::string_view NOT_FOUND = "404 Not Found";
    static constexpr std::string_view METHOD_NOT_ALLOWED = "405 Method Not Allowed";
    static constexpr std::string_view CONFLICT = "409 Conflict";
    static constexpr std::string_view GONE = "410 Gone";
    static constexpr std::string_view PAYLOAD_TOO_LARGE = "413 Payload Too Large";
    static constexpr std::string_view UNSUPPORTED_MEDIA_TYPE = "415 Unsupported Media Type";
    static constexpr std::string_view TOO_MANY_REQUESTS = "429 Too Many Requests";

    static constexpr std::string_view INTERNAL_SERVER_ERROR = "500 Internal Server Error";
    static constexpr std::string_view NOT_IMPLEMENTED = "501 Not Implemented";
    static constexpr std::string_view BAD_GATEWAY = "502 Bad Gateway";
    static constexpr std::string_view SERVICE_UNAVAILABLE = "503 Service Unavailable";
    static constexpr std::string_view GATEWAY_TIMEOUT = "504 Gateway Timeout";
};

struct HttpContentType
{
    static constexpr std::string_view JAVA_ARCHIVE = "application/java-archive";
    static constexpr std::string_view EDI_X12 = "application/EDI-X12";
    static constexpr std::string_view EDIFACT = "application/EDIFACT";
    static constexpr std::string_view APP_JAVASCRIPT = "application/javascript";
    static constexpr std::string_view OCTET_STREAM = "application/octet-stream";
    static constexpr std::string_view OGG = "application/ogg";
    static constexpr std::string_view PDF = "application/pdf";
    static constexpr std::string_view XHTML_XML = "application/xhtml+xml";
    static constexpr std::string_view X_SHOCKWAVE_FLASH = "application/x-shockwave-flash";
    static constexpr std::string_view JSON = "application/json";
    static constexpr std::string_view LD_JSON = "application/ld+json";
    static constexpr std::string_view APP_XML = "application/xml";
    static constexpr std::string_view ZIP = "application/zip";
    static constexpr std::string_view X_WWW_FORM_URLENCODED = "application/x-www-form-urlencoded";

    static constexpr std::string_view MPEG_AUDIO = "audio/mpeg";
    static constexpr std::string_view X_MS_WMA = "audio/x-ms-wma";
    static constexpr std::string_view VND_RN_REALAUDIO = "audio/vnd.rn-realaudio";
    static constexpr std::string_view X_WAV = "audio/x-wav";

    static constexpr std::string_view GIF = "image/gif";
    static constexpr std::string_view JPEG = "image/jpeg";
    static constexpr std::string_view PNG = "image/png";
    static constexpr std::string_view TIFF = "image/tiff";
    static constexpr std::string_view VND_MICROSOFT_ICON = "image/vnd.microsoft.icon";
    static constexpr std::string_view X_ICON = "image/x-icon";
    static constexpr std::string_view VND_DJVU = "image/vnd.djvu";
    static constexpr std::string_view SVG_XML = "image/svg+xml";

    static constexpr std::string_view MULTIPART_MIXED = "multipart/mixed";
    static constexpr std::string_view MULTIPART_ALTERNATIVE = "multipart/alternative";
    static constexpr std::string_view MULTIPART_RELATED = "multipart/related";
    static constexpr std::string_view MULTIPART_FORM_DATA = "multipart/form-data";

    static constexpr std::string_view TEXT = "text/plain";
    static constexpr std::string_view HTML = "text/html";
    static constexpr std::string_view CSS = "text/css";
    static constexpr std::string_view JAVASCRIPT = "text/javascript";
    static constexpr std::string_view CSV = "text/csv";
    static constexpr std::string_view XML = "text/xml";
    static constexpr std::string_view EVENT_STREAM = "text/event-stream";

    static constexpr std::string_view MPEG_VIDEO = "video/mpeg";
    static constexpr std::string_view MP4 = "video/mp4";
    static constexpr std::string_view WEBM = "video/webm";
    static constexpr std::string_view QUICKTIME = "video/quicktime";
    static constexpr std::string_view X_MS_WMV = "video/x-ms-wmv";
    static constexpr std::string_view X_MSVIDEO = "video/x-msvideo";
    static constexpr std::string_view X_FLV = "video/x-flv";

    static constexpr std::string_view VND_ANDROID_PACKAGE_ARCHIVE = "application/vnd.android.package-archive";
    static constexpr std::string_view VND_OASIS_OPENDOCUMENT_TEXT = "application/vnd.oasis.opendocument.text";
    static constexpr std::string_view VND_OASIS_OPENDOCUMENT_SPREADSHEET = "application/vnd.oasis.opendocument.spreadsheet";
    static constexpr std::string_view VND_OASIS_OPENDOCUMENT_PRESENTATION = "application/vnd.oasis.opendocument.presentation";
    static constexpr std::string_view VND_OASIS_OPENDOCUMENT_GRAPHICS = "application/vnd.oasis.opendocument.graphics";
    static constexpr std::string_view VND_MS_EXCEL = "application/vnd.ms-excel";
    static constexpr std::string_view VND_OPENXMLFORMATS_OFFICEDOCUMENT_SPREADSHEETML_SHEET = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    static constexpr std::string_view VND_MS_POWERPOINT = "application/vnd.ms-powerpoint";
    static constexpr std::string_view VND_OPENXMLFORMATS_OFFICEDOCUMENT_PRESENTATIONML_PRESENTATION = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    static constexpr std::string_view MSWORD = "application/msword";
    static constexpr std::string_view VND_OPENXMLFORMATS_OFFICEDOCUMENT_WORDPROCESSINGML_DOCUMENT = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    static constexpr std::string_view VND_MOZILLA_XUL_XML = "application/vnd.mozilla.xul+xml";
};

struct HttpResponse
{
    std::string http_version{ HttpVersion::_11 };
    std::string status_code{ HttpStatusCode::OK };
    std::map<std::string, std::string> headers;
    std::string content_type{ HttpContentType::TEXT };
    std::string content;

    HttpResponse() = default;

    void load_text( std::string_view const& text );
    void load_html( Html const& html );
    void load_raw( void const* ptr, uint64_t byte_size );
    void load_file( fs::path const& path, std::optional<ByteRange> const& byte_range );

    template<typename... Args>
    void load( Args&&... args )
    {
        load_text( format( args... ) );
    }

    std::string compile() const;
};

std::optional<std::string> probe_content_type( fs::path const& path );
}
