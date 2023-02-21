#include "graphics/shaders/compiled_shader.h"


// Creation
kl::compiled_shader::compiled_shader()
{}

kl::compiled_shader::~compiled_shader()
{}

// Get
kl::compiled_shader::operator bool() const
{
    return (data && !error);
}

const void* kl::compiled_shader::get_data() const
{
    return data ? (const void*) data->GetBufferPointer() : nullptr;
}

SIZE_T kl::compiled_shader::get_data_size() const
{
    return data ? data->GetBufferSize() : 0;
}

std::string kl::compiled_shader::get_error() const
{
    return error ? ((const char*) error->GetBufferPointer()) : "Unknown";
}

SIZE_T kl::compiled_shader::get_error_size() const
{
    return error ? error->GetBufferSize() : 0;
}
