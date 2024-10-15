#include "klibrary.h"


kl::CompiledShader kl::ShaderCompiler::compile(const std::string_view& name, const std::string_view& version, const std::string_view& source) const
{
    static constexpr UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
    CompiledShader compiled_shader{};
    D3DCompile(source.data(), source.size(), nullptr, nullptr, nullptr, name.data(), version.data(),
        flags, NULL, &compiled_shader.data, &compiled_shader.error);
    verify(compiled_shader, name, ": ", compiled_shader.error_val());
    return compiled_shader;
}

kl::CompiledShader kl::ShaderCompiler::compile_vertex_shader(const std::string_view& source) const
{
    return compile("v_shader", "vs_5_0", source);
}

kl::CompiledShader kl::ShaderCompiler::compile_geometry_shader(const std::string_view& source) const
{
    return compile("g_shader", "gs_5_0", source);
}

kl::CompiledShader kl::ShaderCompiler::compile_pixel_shader(const std::string_view& source) const
{
    return compile("p_shader", "ps_5_0", source);
}

kl::CompiledShader kl::ShaderCompiler::compile_compute_shader(const std::string_view& source) const
{
    return compile("c_shader", "cs_5_0", source);
}
