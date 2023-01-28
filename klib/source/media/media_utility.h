#pragma once

#include "window/system_handler.h"

#include <string>


namespace kl::media_utility {
    void fail_check_(HRESULT hr, const std::string& error_message);
    bool succeeded_(HRESULT hr);

    template<class T> void safe_release_(T*& data)
    {
        if (data) {
            data->Release();
            data = nullptr;
        }
    }

    void init();
    void uninit();
}
