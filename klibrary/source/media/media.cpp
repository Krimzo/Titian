#include "klibrary.h"


static const int _media_init = []
{
    constexpr DWORD init_args = COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY;
    CoInitializeEx(nullptr, init_args) >> kl::verify_result;
    MFStartup(MF_VERSION) >> kl::verify_result;
    return 0;
}();
