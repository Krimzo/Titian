#pragma once

#include "apis/apis.h"


namespace kl {
    struct compiled_shader
    {
        dx::data_blob m_data = nullptr;
        dx::data_blob m_error = nullptr;

        operator bool() const;

        const void* data() const;
        SIZE_T data_size() const;

        std::string error() const;
        SIZE_T error_size() const;
    };
}
