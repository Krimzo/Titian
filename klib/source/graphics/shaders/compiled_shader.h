#pragma once

#include "apis/apis.h"


namespace kl {
    struct compiled_shader
    {
        dx::data_blob data = nullptr;
        dx::data_blob error = nullptr;

        operator bool() const;

        const void* get_data() const;
        SIZE_T get_data_size() const;

        std::string get_error() const;
        SIZE_T get_error_size() const;
    };
}
