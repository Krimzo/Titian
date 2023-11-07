#include "klibrary.h"


// Compilation
kl::CompiledShader kl::ShaderCompiler::compile(const std::string& name, const std::string& version, const std::string& source) const
{
    CompiledShader compiled_shader = {};
    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, name.c_str(), version.c_str(), NULL, NULL, &compiled_shader.data, &compiled_shader.error);
    verify(compiled_shader, kl::format(name, ": ", compiled_shader.error_val()));
    return compiled_shader;
}

// Predefined compilation
kl::CompiledShader kl::ShaderCompiler::compile_vertex_shader(const std::string& source) const
{
    return compile("v_shader", "vs_5_0", source);
}

kl::CompiledShader kl::ShaderCompiler::compile_geometry_shader(const std::string& source) const
{
    return compile("g_shader", "gs_5_0", source);
}

kl::CompiledShader kl::ShaderCompiler::compile_pixel_shader(const std::string& source) const
{
    return compile("p_shader", "ps_5_0", source);
}

kl::CompiledShader kl::ShaderCompiler::compile_compute_shader(const std::string& source) const
{
    return compile("c_shader", "cs_5_0", source);
}
