export module java;

export import klib;
export import "jni.h";

#undef VOID
#pragma comment(lib, "jvm.lib")

export using jvoid = void;
export using jbool = jboolean;
export using jfield = jfieldID;
export using jmethod = jmethodID;

export namespace java {
    inline const std::string BOOL = "Z";
    inline const std::string BYTE = "B";

    inline const std::string CHAR = "C";
    inline const std::string SHORT = "S";

    inline const std::string INT = "I";
    inline const std::string FLOAT = "F";

    inline const std::string LONG = "J";
    inline const std::string DOUBLE = "D";

    inline const std::string VOID = "V";
    inline const std::string CONSTRUCTOR = "<init>";
}

export namespace java {
    std::string build_class_signature(const std::string& type)
    {
        return ("L" + type + ";");
    }

    std::string build_array_signature(const std::string& type)
    {
        return ("[" + type);
    }

    std::string build_method_signature(const std::string& return_type, const std::vector<std::string>& parameters)
    {
        std::stringstream stream = {};
        stream << "(";
        for (auto& parameter : parameters) {
            stream << parameter;
        }
        stream << ")" << return_type;
        return stream.str();
    }
}

export namespace java {
    inline JavaVM* jvm = nullptr;
    inline JNIEnv* jni = nullptr;

    jint init(const std::string& dll_path, std::vector<std::string> args)
    {
        if (!LoadLibraryA(dll_path.c_str())) {
            return false;
        }

        std::vector<JavaVMOption> options{ args.size() };
        for (size_t i = 0; i < options.size(); i++) {
            options[i].optionString = args[i].data();
        }

        JavaVMInitArgs jvm_args = {};
        jvm_args.version = JNI_VERSION_20;
        jvm_args.nOptions = (jint) options.size();
        jvm_args.options = options.data();

        return JNI_CreateJavaVM(&jvm, (void**) &jni, &jvm_args);
    }
}

export namespace java {
    jmethod get_method(jclass clazz, const std::string& name, const std::string& return_type = VOID, const std::vector<std::string>& parameters = {})
    {
        const std::string signature = build_method_signature(return_type, parameters);
        return jni->GetMethodID(clazz, name.c_str(), signature.c_str());
    }

    jmethod get_constructor(jclass clazz, const std::vector<std::string>& parameters = {})
    {
        return get_method(clazz, CONSTRUCTOR, VOID, parameters);
    }

    jmethod get_static_method(jclass clazz, const std::string& name, const std::string& return_type = VOID, const std::vector<std::string>& parameters = {})
    {
        const std::string signature = build_method_signature(return_type, parameters);
        return jni->GetStaticMethodID(clazz, name.c_str(), signature.c_str());
    }
}

export namespace java {
    jobject new_object(jclass clazz, jmethod constructor, ...)
    {
        va_list args = {};
        va_start(args, constructor);
        jobject object = jni->NewObjectV(clazz, constructor, args);
        va_end(args);
        return object;
    }

    void delete_object(jobject object)
    {
        if (object) {
            jni->DeleteLocalRef(object);
        }
    }
}

export namespace java {
    using class_data = std::vector<byte>;

    class_data read_class_data(const std::string& path)
    {
        FILE* file = nullptr;
        if (fopen_s(&file, path.c_str(), "rb")) {
            return {};
        }

        fseek(file, 0, SEEK_END);
        class_data result(ftell(file));

        rewind(file);
        fread(result.data(), 1, result.size(), file);

        fclose(file);
        return result;
    }

    jclass load_eternal_class(const std::string& name)
    {
        return jni->FindClass(name.c_str());
    }

    jclass load_class(jobject loader, const class_data& data)
    {
        return jni->DefineClass(nullptr, loader, (const jbyte*) data.data(), (jsize) data.size());
    }

    jclass load_eternal_class(const class_data& data)
    {
        return load_class(nullptr, data);
    }

    jobject new_loader()
    {
        static const jclass loader_class = java::load_eternal_class(java::class_data{
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
        return java::new_object(loader_class, loader_constructor);
    }
}

export namespace java {
    jstring new_string(const std::string& data)
    {
        return jni->NewStringUTF(data.c_str());
    }

    std::string read_string(jstring object)
    {
        jbool ignored = false;
        return { jni->GetStringUTFChars(object, &ignored) };
    }
}
