#include "klibrary.h"


kl::CompiledShader::operator bool() const
{
    return data && !error;
}

void const* kl::CompiledShader::data_ptr() const
{
    return data ? (void*) data->GetBufferPointer() : nullptr;
}

SIZE_T kl::CompiledShader::data_size() const
{
    return data ? data->GetBufferSize() : 0;
}

char const* kl::CompiledShader::error_val() const
{
    return error ? (char*) error->GetBufferPointer() : nullptr;
}

SIZE_T kl::CompiledShader::error_size() const
{
    return error ? error->GetBufferSize() : 0;
}

kl::CompiledShader kl::ShaderCompiler::compile( std::string_view const& name, std::string_view const& version, std::string_view const& source ) const
{
    static constexpr UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
    CompiledShader compiled_shader{};
    D3DCompile( source.data(), source.size(), nullptr, nullptr, nullptr, name.data(), version.data(),
        flags, NULL, &compiled_shader.data, &compiled_shader.error );
    verify( compiled_shader, name, ": ", compiled_shader.error_val() );
    return compiled_shader;
}

kl::CompiledShader kl::ShaderCompiler::compile_vertex_shader( std::string_view const& source ) const
{
    return compile( "v_shader", "vs_5_0", source );
}

kl::CompiledShader kl::ShaderCompiler::compile_geometry_shader( std::string_view const& source ) const
{
    return compile( "g_shader", "gs_5_0", source );
}

kl::CompiledShader kl::ShaderCompiler::compile_pixel_shader( std::string_view const& source ) const
{
    return compile( "p_shader", "ps_5_0", source );
}

kl::CompiledShader kl::ShaderCompiler::compile_compute_shader( std::string_view const& source ) const
{
    return compile( "c_shader", "cs_5_0", source );
}
