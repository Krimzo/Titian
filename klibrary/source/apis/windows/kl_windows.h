#pragma once

// Web
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// Media
#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <propvarutil.h>

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "propsys.lib")

// Windows
#include <windows.h>
#include <windowsx.h>
#include <wininet.h>
#include <conio.h>

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

namespace kl {
	/*
	*	Thanks CHILI!
	*/

	struct VerifyResult
	{};

	struct ResultGrabber
	{
		const HRESULT result{};
		const std::source_location location{};

		inline constexpr ResultGrabber(const HRESULT result, std::source_location location = std::source_location::current()) noexcept
			: result(result), location(location)
		{}
	};

	inline void operator>>(const ResultGrabber& grabber, const VerifyResult& token)
	{
		static constexpr auto get_error_description = [](const HRESULT h_result) -> std::string
		{
			char* description_buffer = nullptr;
			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, h_result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&description_buffer), 0, nullptr
			);

			std::string description{};
			if (description_buffer) {
				description = description_buffer;
				LocalFree(description_buffer);
			}
			return description;
		};

		if (FAILED(grabber.result)) {
			const std::string message = std::format("Result error: {}\n{}({})",
				get_error_description(grabber.result),
				grabber.location.file_name(),
				grabber.location.line()
			);
			MessageBoxA(nullptr, message.data(), "Assertion failed!", MB_ICONERROR | MB_OK);
			exit(static_cast<int>(grabber.result));
		}
	}

	inline constexpr VerifyResult verify_result{};
}
