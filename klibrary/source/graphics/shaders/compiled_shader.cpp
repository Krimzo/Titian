#include "klibrary.h"


kl::CompiledShader::operator bool() const
{
    return (data && !error);
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
