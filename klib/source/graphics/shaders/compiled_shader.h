#pragma once

#include "apis/apis.h"


namespace kl {
    struct compiled_shader
    {
        dx::shader_blob data = nullptr;
        dx::shader_blob error = nullptr;

        // Creation
        compiled_shader();
        ~compiled_shader();

        // Get
        operator bool() const;

        const void* get_data() const;
        SIZE_T get_data_size() const;

        std::string get_error() const;
        SIZE_T get_error_size() const;
    };
}
