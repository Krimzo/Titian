#pragma once


// CPP
#include "apis/cpp/kl_cpp.h"

// Windows
#include "apis/windows/kl_windows.h"
#include "apis/windows/kl_directx.h"

// External
#include "apis/external/kl_dx12.h"
#include "apis/external/kl_imgui.h"

namespace kl {
    inline constexpr bool IS_DEBUG =
#ifdef _DEBUG
        true;
#else
        false;
#endif
}
