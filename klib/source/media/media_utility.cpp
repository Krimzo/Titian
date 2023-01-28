#include "media/media_utility.h"

#include "utility/console.h"
#include <atomic>


static std::atomic<bool> utility_initialised = false;

void kl::media_utility::fail_check_(HRESULT hr, const std::string& error_message)
{
    kl::error_check(FAILED(hr), error_message);
}

bool kl::media_utility::succeeded_(HRESULT hr)
{
    return SUCCEEDED(hr);
}

void kl::media_utility::init()
{
    static constexpr DWORD init_args = COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY;

    if (!utility_initialised) {
        fail_check_(CoInitializeEx(nullptr, init_args), "Failed to initialize media utility [CoInitializeEx]");
        fail_check_(MFStartup(MF_VERSION), "Failed to initialize media utility [MFStartup]");
        utility_initialised = true;
    }
}

void kl::media_utility::uninit()
{
    if (utility_initialised) {
        MFShutdown();
        CoUninitialize();
        utility_initialised = false;
    }
}
