#include "java.h"


// Signatures
std::string java::build_class_signature(const std::string& type)
{
	return ("L" + type + ";");
}

std::string java::build_array_signature(const std::string& type)
{
	return ("[" + type);
}

std::string java::build_method_signature(const std::string& return_type, const std::vector<std::string>& parameters)
{
	std::stringstream stream = {};
	stream << "(";
	for (auto& parameter : parameters) {
		stream << parameter;
	}
	stream << ")" << return_type;
	return stream.str();
}

// Init
bool java::init(const std::string& dll_path, std::vector<std::string> args)
{
	if (!LoadLibraryA(dll_path.c_str())) {
		return false;
	}

	std::vector<JavaVMOption> options { args.size() };
	for (size_t i = 0; i < options.size(); i++) {
		options[i].optionString = args[i].data();
	}

	JavaVMInitArgs jvm_args = {};
	jvm_args.version = JNI_VERSION_20;
	jvm_args.nOptions = (jint) options.size();
	jvm_args.options = options.data();

	return !JNI_CreateJavaVM(&jvm, (void**) &jni, &jvm_args);
}

// Loading
jclass java::load_eternal_class(const std::string& name)
{
	return jni->FindClass(name.c_str());
}

jclass java::load_eternal_class(const clazz_data& data)
{
	return load_class(nullptr, data);
}

jclass java::load_class(jobject loader, const clazz_data& data)
{
	return jni->DefineClass(nullptr, loader, (const jbyte*) data.data(), (jsize) data.size());
}

// Methods
jmethod java::get_method(jclass clazz, const std::string& name, const std::string& return_type, const std::vector<std::string>& parameters)
{
	const std::string signature = build_method_signature(return_type, parameters);
	return jni->GetMethodID(clazz, name.c_str(), signature.c_str());
}

jmethod java::get_static_method(jclass clazz, const std::string& name, const std::string& return_type, const std::vector<std::string>& parameters)
{
	const std::string signature = build_method_signature(return_type, parameters);
	return jni->GetStaticMethodID(clazz, name.c_str(), signature.c_str());
}

jmethod java::get_constructor(jclass clazz, const std::vector<std::string>& parameters)
{
	return get_method(clazz, CONSTRUCT, VOID, parameters);
}

// Objects
jobject java::new_object(jclass clazz, jmethod constructor, ...)
{
	va_list args = {};
	va_start(args, constructor);
	jobject object = jni->NewObjectV(clazz, constructor, args);
	va_end(args);
	return object;
}

void java::delete_object(jobject object)
{
	if (object) {
		jni->DeleteLocalRef(object);
	}
}

// Strings
jstring java::new_string(const std::string& data)
{
	return jni->NewStringUTF(data.c_str());
}

std::string java::read_string(jstring object)
{
	jbool ignored = false;
	return { jni->GetStringUTFChars(object, &ignored) };
}
