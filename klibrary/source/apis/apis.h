#pragma once

#include "apis/kl_cpp.h"
#include "apis/kl_windows.h"
#include "apis/kl_directx.h"
#include "apis/kl_imgui.h"


namespace kl {
    inline constexpr bool IS_DEBUG =
#ifdef _DEBUG
        true;
#else
        false;
#endif
}
