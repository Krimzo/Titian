#include "jdk.h"
#include "klib.h"


static const std::string class_paths[1] = { "." };

void jdk::init()
{
	// Load DLL
	HMODULE jvm_dll = LoadLibraryA("jdk/bin/server/jvm.dll");
	if (!jvm_dll) {
		kl::print("Failed to load jvm.dll");
		exit(1);
	}

	// Main VM args
	JavaVMInitArgs vm_args = {};
	vm_args.version = JNI_VERSION_20;

	// Generate arguments string
	std::stringstream jvm_options_stream = {};
	jvm_options_stream << "-Djava.class.path=";
	for (const auto& path : class_paths) {
		jvm_options_stream << path << ";";
	}
	const std::string jvm_options_data = jvm_options_stream.str();

	// Create jvm options
	JavaVMOption jvm_options[1] = {};
	jvm_options[0].optionString = (char*) jvm_options_data.c_str();

	// Pass jvm options
	vm_args.options = jvm_options;
	vm_args.nOptions = (jint) std::size(jvm_options);

	// Create jni
	jint result = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);
	if (result != JNI_OK) {
		kl::print("Failed to init jvm, result: ", result);
		exit((int) result);
	}
}

void jdk::uninit()
{
	jvm->DestroyJavaVM();
	jvm = nullptr;
	env = nullptr;
}
