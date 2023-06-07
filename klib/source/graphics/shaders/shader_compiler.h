#pragma once

#include "graphics/shaders/compiled_shader.h"


namespace kl {
    class shader_compiler
    {
    public:
        // Compilation
        compiled_shader compile(const std::string& name, const std::string& version, const std::string& source) const;

        // Predefined compilation
        compiled_shader compile_vertex_shader(const std::string& source) const;
        compiled_shader compile_geometry_shader(const std::string& source) const;
        compiled_shader compile_pixel_shader(const std::string& source) const;
        compiled_shader compile_compute_shader(const std::string& source) const;
    };
}
