#pragma once

#include "klib.h"
#include <jni.h>

#undef VOID

#pragma comment(lib, "jvm.lib")


using jvoid = void;
using jbool = jboolean;
using jmethod = jmethodID;

namespace java {
    using clazz_data = std::vector<uint8_t>;
}

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
}

namespace java {
    // Setup
    bool init(const std::string& dll_path, std::vector<std::string> args);
    
    // Loading
    jclass load_eternal_class(const std::string& name);
    jclass load_eternal_class(const clazz_data& data);
    jclass load_class(jobject loader, const clazz_data& data);

    // Methods
    jmethod get_method(jclass clazz, const std::string& name, const std::string& return_type = VOID, const std::vector<std::string>& parameters = {});
    jmethod get_static_method(jclass clazz, const std::string& name, const std::string& return_type = VOID, const std::vector<std::string>& parameters = {});
    jmethod get_constructor(jclass clazz, const std::vector<std::string>& parameters = {});

    // Objects
    jobject new_object(jclass clazz, jmethod constructor, ...);
    void delete_object(jobject object);

    // Strings
    jstring new_string(const std::string& data);
    std::string read_string(jstring object);
}
