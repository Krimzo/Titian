#include "klibrary.h"


kl::CompiledShader::operator bool() const
{
    return data && !error;
}

const void* kl::CompiledShader::data_ptr() const
{
    return data ? (const void*) data->GetBufferPointer() : nullptr;
}

SIZE_T kl::CompiledShader::data_size() const
{
    return data ? data->GetBufferSize() : 0;
}

const char* kl::CompiledShader::error_val() const
{
    return (const char*) (error ? error->GetBufferPointer() : nullptr);
}

SIZE_T kl::CompiledShader::error_size() const
{
    return error ? error->GetBufferSize() : 0;
}

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
