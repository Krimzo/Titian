#pragma once

#define NOMINMAX

#include <ws2tcpip.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <propvarutil.h>
#include <audioclient.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <codecapi.h>
#include <dvdmedia.h>
#include <windows.h>
#include <windowsx.h>
#include <wininet.h>
#include <dwmapi.h>
#include <conio.h>
#include <shlobj.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "propsys.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "avrt.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "dwmapi.lib")


namespace kl
{
struct VerifyResult
{};

struct ResultGrabber
{
    HRESULT result{};
    std::source_location location{};

    ResultGrabber( HRESULT result, std::source_location location = std::source_location::current() ) noexcept
        : result( result ), location( location )
    {}
};

inline std::wstring get_error_description( HRESULT h_result )
{
    LPWSTR description_buffer = nullptr;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, h_result, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPWSTR) &description_buffer, 0, nullptr );
    std::wstring description{};
    if ( description_buffer )
    {
        description = description_buffer;
        LocalFree( description_buffer );
    }
    return description;
}

inline std::function<void( ResultGrabber const& )> VERIFY_RESULT_FAIL_FUNC = []( ResultGrabber const& grabber ) -> void
    {
        std::wstringstream stream;
        stream << grabber.location.file_name() << "(" << grabber.location.line() << ")\n" << get_error_description( grabber.result );
        MessageBoxW( nullptr, stream.str().data(), L"Failed to verify HRESULT.", MB_ICONERROR | MB_OK );
        std::exit( grabber.result );
    };

inline void operator>>( ResultGrabber const& grabber, VerifyResult const& _ )
{
    if ( FAILED( grabber.result ) )
        VERIFY_RESULT_FAIL_FUNC( grabber );
}

inline constexpr VerifyResult verify_result{};
}
