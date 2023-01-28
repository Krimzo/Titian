#pragma once

// Windows includes
#include <ws2tcpip.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <propvarutil.h>

#include <windows.h>
#include <windowsx.h>
#include <wininet.h>
#include <conio.h>

#include <gdiplus.h>
#include <d3dcompiler.h>
#include <d3d11.h>

// Undef windows garbage defines
#undef byte
#undef BYTE

#undef min
#undef max

#undef near
#undef far

// Linking libs
#pragma comment(lib, "Ws2_32.lib")

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "propsys.lib")

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
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

#pragma comment(lib, "gdiplus.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3d11.lib")
