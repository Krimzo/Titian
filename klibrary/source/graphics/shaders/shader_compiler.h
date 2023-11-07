#pragma once

#include "graphics/shaders/compiled_shader.h"


namespace kl {
    class ShaderCompiler
    {
    public:
        // Compilation
        CompiledShader compile(const std::string& name, const std::string& version, const std::string& source) const;

        // Predefined compilation
        CompiledShader compile_vertex_shader(const std::string& source) const;
        CompiledShader compile_geometry_shader(const std::string& source) const;
        CompiledShader compile_pixel_shader(const std::string& source) const;
        CompiledShader compile_compute_shader(const std::string& source) const;
    };
}
