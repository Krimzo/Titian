#include "java.h"
#include "java_clazz.h"


static java::clazz_data read_class_file(const std::string& path)
{
    FILE* file = nullptr;
    if (fopen_s(&file, path.c_str(), "rb")) {
        return {};
    }

    fseek(file, 0, SEEK_END);
    java::clazz_data result(ftell(file));

    rewind(file);
    fread(result.data(), 1, result.size(), file);

    fclose(file);
    return result;
}

java::clazz::clazz(const std::string& path)
    : clazz(read_class_file(path))
{}

java::clazz::clazz(const clazz_data& data)
{
    static const jclass loader_class = java::load_eternal_class(java::clazz_data {
        0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x10, 0x0A, 0x00, 0x02, 0x00,
        0x03, 0x07, 0x00, 0x04, 0x0C, 0x00, 0x05, 0x00, 0x06, 0x01, 0x00, 0x15, 0x6A, 0x61,
        0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x4C,
        0x6F, 0x61, 0x64, 0x65, 0x72, 0x01, 0x00, 0x06, 0x3C, 0x69, 0x6E, 0x69, 0x74, 0x3E,
        0x01, 0x00, 0x03, 0x28, 0x29, 0x56, 0x07, 0x00, 0x08, 0x01, 0x00, 0x0A, 0x53, 0x61,
        0x6E, 0x65, 0x4C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x01, 0x00, 0x04, 0x43, 0x6F, 0x64,
        0x65, 0x01, 0x00, 0x0F, 0x4C, 0x69, 0x6E, 0x65, 0x4E, 0x75, 0x6D, 0x62, 0x65, 0x72,
        0x54, 0x61, 0x62, 0x6C, 0x65, 0x01, 0x00, 0x12, 0x4C, 0x6F, 0x63, 0x61, 0x6C, 0x56,
        0x61, 0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x54, 0x61, 0x62, 0x6C, 0x65, 0x01, 0x00,
        0x04, 0x74, 0x68, 0x69, 0x73, 0x01, 0x00, 0x0C, 0x4C, 0x53, 0x61, 0x6E, 0x65, 0x4C,
        0x6F, 0x61, 0x64, 0x65, 0x72, 0x3B, 0x01, 0x00, 0x0A, 0x53, 0x6F, 0x75, 0x72, 0x63,
        0x65, 0x46, 0x69, 0x6C, 0x65, 0x01, 0x00, 0x0F, 0x53, 0x61, 0x6E, 0x65, 0x4C, 0x6F,
        0x61, 0x64, 0x65, 0x72, 0x2E, 0x6A, 0x61, 0x76, 0x61, 0x00, 0x21, 0x00, 0x07, 0x00,
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x05, 0x00, 0x06, 0x00,
        0x01, 0x00, 0x09, 0x00, 0x00, 0x00, 0x2F, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x05, 0x2A, 0xB7, 0x00, 0x01, 0xB1, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0A, 0x00, 0x00,
        0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C,
        0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x0E, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0F,
    });
    static const jmethod loader_constructor = java::get_constructor(loader_class);

    m_loader = java::new_object(loader_class, loader_constructor);
    m_clazz = java::load_class(m_loader, data);
}

java::clazz::~clazz()
{
    java::delete_object(m_clazz);
    java::delete_object(m_loader);
}

// Get
java::clazz::operator jclass() const
{
    return m_clazz;
}

java::clazz::operator bool() const
{
    return (bool) m_clazz;
}

// Methods
jmethod java::clazz::get_method(const std::string& name, const std::string& return_type, const std::vector<std::string>& parameters)
{
    return java::get_method(m_clazz, name, return_type, parameters);
}

jmethod java::clazz::get_static_method(const std::string& name, const std::string& return_type, const std::vector<std::string>& parameters)
{
    return java::get_static_method(m_clazz, name, return_type, parameters);
}

jmethod java::clazz::get_constructor(const std::vector<std::string>& parameters)
{
    return java::get_constructor(m_clazz, parameters);
}

// Objects
jobject java::clazz::new_object(jmethod constructor, ...)
{
    va_list args = {};
    va_start(args, constructor);
    jobject object = jni->NewObjectV(m_clazz, constructor, args);
    va_end(args);
    return object;
}
