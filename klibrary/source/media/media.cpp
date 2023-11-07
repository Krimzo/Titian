#include "klibrary.h"


// Init
namespace kl_ignored {
    static const int DONT_CARE = []()
    {
        constexpr DWORD init_args = (COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
        CoInitializeEx(nullptr, init_args) >> kl::hr_checker;
        MFStartup(MF_VERSION) >> kl::hr_checker;
        return 0;
    }();
}
