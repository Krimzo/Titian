#include "java.h"
#include "java_object.h"


java::object::object(jobject object)
    : m_object(object)
{}

java::object::~object()
{
    java::delete_object(m_object);
}

// Get
java::object::operator jobject() const
{
    return m_object;
}

java::object::operator bool() const
{
    return (bool) m_object;
}
