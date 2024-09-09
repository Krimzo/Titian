#pragma once

#include "apis/apis.h"


namespace kl {
    struct CompiledShader
    {
        dx::DataBlob data;
        dx::DataBlob error;
        
        operator bool() const;

        const void* data_ptr() const;
        SIZE_T data_size() const;

        std::string error_val() const;
        SIZE_T error_size() const;
    };
}
