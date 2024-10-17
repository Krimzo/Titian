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

        const char* error_val() const;
        SIZE_T error_size() const;
    };
}

namespace kl {
    struct ShaderCompiler
    {
        CompiledShader compile(const std::string_view& name, const std::string_view& version, const std::string_view& source) const;
        CompiledShader compile_vertex_shader(const std::string_view& source) const;
        CompiledShader compile_geometry_shader(const std::string_view& source) const;
        CompiledShader compile_pixel_shader(const std::string_view& source) const;
        CompiledShader compile_compute_shader(const std::string_view& source) const;
    };
}
