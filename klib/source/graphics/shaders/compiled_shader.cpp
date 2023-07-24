#include "graphics/shaders/compiled_shader.h"


kl::compiled_shader::operator bool() const
{
    return (m_data && !m_error);
}

const void* kl::compiled_shader::data() const
{
    return m_data ? (const void*) m_data->GetBufferPointer() : nullptr;
}

SIZE_T kl::compiled_shader::data_size() const
{
    return m_data ? m_data->GetBufferSize() : 0;
}

std::string kl::compiled_shader::error() const
{
    return m_error ? ((const char*) m_error->GetBufferPointer()) : "Unknown";
}

SIZE_T kl::compiled_shader::error_size() const
{
    return m_error ? m_error->GetBufferSize() : 0;
}
