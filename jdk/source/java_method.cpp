#include "java.h"
#include "java_method.h"


java::method::method(jmethod method)
    : m_method(method)
{}

// Get
java::method::operator jmethod() const
{
    return m_method;
}

java::method::operator bool() const
{
    return (bool) m_method;
}
