#pragma once

#include "apis/apis.h"


namespace kl {
    class CompiledShader
    {
    public:
        dx::DataBlob data;
        dx::DataBlob error;
        
        operator bool() const;

        const void* data_val() const;
        SIZE_T data_size() const;

        std::string error_val() const;
        SIZE_T error_size() const;
    };
}
