#pragma once

#include "klib.h"
#include <jni.h>

#undef VOID

#pragma comment(lib, "jvm.lib")


using jvoid = void;
using jbool = jboolean;
using jfield = jfieldID;
using jmethod = jmethodID;

namespace java {
    inline const std::string BOOL = "Z";
    inline const std::string BYTE = "B";

    inline const std::string CHAR = "C";
    inline const std::string SHORT = "S";

    inline const std::string INT = "I";
    inline const std::string FLOAT = "F";

    inline const std::string LONG = "J";
    inline const std::string DOUBLE = "D";

    inline const std::string VOID = "V";
    inline const std::string CONSTRUCT = "<init>";
}

namespace java {
    std::string build_class_signature(const std::string& type);
    std::string build_array_signature(const std::string& type);
    std::string build_method_signature(const std::string& return_type, const std::vector<std::string>& parameters);
}

namespace java {
    inline JavaVM* jvm = nullptr;
    inline JNIEnv* jni = nullptr;

    bool init(const std::string& dll_path, std::vector<std::string> args);
}

namespace java {
    using class_data = std::vector<byte>;

    jobject new_loader();
    class_data read_class_data(const std::string& path);

    jclass load_eternal_class(const std::string& name);
    jclass load_eternal_class(const class_data& data);

    jclass load_class(jobject loader, const class_data& data);
}

namespace java {
    jmethod get_method(jclass clazz, const std::string& name, const std::string& return_type = VOID, const std::vector<std::string>& parameters = {});
    jmethod get_static_method(jclass clazz, const std::string& name, const std::string& return_type = VOID, const std::vector<std::string>& parameters = {});
    jmethod get_constructor(jclass clazz, const std::vector<std::string>& parameters = {});
}

namespace java {
    jobject new_object(jclass clazz, jmethod constructor, ...);
    void delete_object(jobject object);
}

namespace java {
    jstring new_string(const std::string& data);
    std::string read_string(jstring object);
}
