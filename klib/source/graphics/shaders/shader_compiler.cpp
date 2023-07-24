#include "graphics/shaders/shader_compiler.h"

#include "utility/utility.h"


// Compilation
kl::compiled_shader kl::shader_compiler::compile(const std::string& name, const std::string& version, const std::string& source) const
{
    compiled_shader compiled_shader = {};
    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, name.c_str(), version.c_str(), NULL, NULL, &compiled_shader.m_data, &compiled_shader.m_error);
    warning_check(!compiled_shader, kl::format(name, ": ", compiled_shader.error()));
    return compiled_shader;
}

// Predefined compilation
kl::compiled_shader kl::shader_compiler::compile_vertex_shader(const std::string& source) const
{
    return compile("v_shader", "vs_5_0", source);
}

kl::compiled_shader kl::shader_compiler::compile_geometry_shader(const std::string& source) const
{
    return compile("g_shader", "gs_5_0", source);
}

kl::compiled_shader kl::shader_compiler::compile_pixel_shader(const std::string& source) const
{
    return compile("p_shader", "ps_5_0", source);
}

kl::compiled_shader kl::shader_compiler::compile_compute_shader(const std::string& source) const
{
    return compile("c_shader", "cs_5_0", source);
}
