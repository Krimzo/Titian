#pragma once

#include "apis/apis.h"


namespace kl
{
struct CompiledShader
{
    dx::DataBlob data;
    dx::DataBlob error;

    operator bool() const;

    void const* data_ptr() const;
    SIZE_T data_size() const;

    char const* error_val() const;
    SIZE_T error_size() const;
};
}

namespace kl
{
struct ShaderCompiler
{
    CompiledShader compile( std::string_view const& name, std::string_view const& version, std::string_view const& source ) const;
    CompiledShader compile_vertex_shader( std::string_view const& source ) const;
    CompiledShader compile_geometry_shader( std::string_view const& source ) const;
    CompiledShader compile_pixel_shader( std::string_view const& source ) const;
    CompiledShader compile_compute_shader( std::string_view const& source ) const;
};
}
