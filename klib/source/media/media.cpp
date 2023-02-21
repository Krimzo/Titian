#include "media/media.h"

#include "utility/utility.h"


// Init
namespace kl_ignored {
    static const int dont_care = []()
    {
        const DWORD init_args = (COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
        kl::media_utility::fail_check_(CoInitializeEx(nullptr, init_args), "Failed to initialize media utility [CoInitializeEx]");
        kl::media_utility::fail_check_(MFStartup(MF_VERSION), "Failed to initialize media utility [MFStartup]");
        return 0;
    }();
}

// Utility
void kl::media_utility::fail_check_(HRESULT hr, const std::string& error_message)
{
    kl::error_check(FAILED(hr), error_message);
}

bool kl::media_utility::succeeded_(HRESULT hr)
{
    return SUCCEEDED(hr);
}
