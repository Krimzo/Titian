#pragma once

#include "java_value.h"


namespace java {
    class method
    {
        jmethod m_method = nullptr;

    public:
        method(jmethod method);

        method(const method&) = delete;
        method(const method&&) = delete;

        void operator=(const method&) = delete;
        void operator=(const method&&) = delete;

        // Get
        operator jmethod() const;
        operator bool() const;

        // Call
		template<typename R = jvoid>
		R call(jobject instance, const std::initializer_list<java::value>& args = {})
		{
			const jvalue* arg_data = reinterpret_cast<const jvalue*>(args.begin());
			if constexpr (std::is_same_v<R, jvoid>) {
				jni->CallVoidMethodA(instance, m_method, arg_data);
				return;
			}
			if constexpr (std::is_same_v<R, jbool>) {
				return jni->CallBooleanMethodA(instance, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jbyte>) {
				return jni->CallByteMethodA(instance, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jchar>) {
				return jni->CallCharMethodA(instance, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jshort>) {
				return jni->CallShortMethodA(instance, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jint>) {
				return jni->CallIntMethodA(instance, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jlong>) {
				return jni->CallLongMethodA(instance, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jfloat>) {
				return jni->CallFloatMethodA(instance, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jdouble>) {
				return jni->CallDoubleMethodA(instance, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jobject>) {
				return jni->CallObjectMethodA(instance, m_method, arg_data);
			}
			throw std::exception("Unsupported return type");
		}

        template<typename R = jvoid>
		R call_static(const std::initializer_list<java::value>& args = {})
        {
			const jvalue* arg_data = reinterpret_cast<const jvalue*>(args.begin());
			if constexpr (std::is_same_v<R, jvoid>) {
				jni->CallStaticVoidMethodA(nullptr, m_method, arg_data);
				return;
			}
			if constexpr (std::is_same_v<R, jbool>) {
				return jni->CallStaticBooleanMethodA(nullptr, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jbyte>) {
				return jni->CallStaticByteMethodA(nullptr, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jchar>) {
				return jni->CallStaticCharMethodA(nullptr, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jshort>) {
				return jni->CallStaticShortMethodA(nullptr, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jint>) {
				return jni->CallStaticIntMethodA(nullptr, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jlong>) {
				return jni->CallStaticLongMethodA(nullptr, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jfloat>) {
				return jni->CallStaticFloatMethodA(nullptr, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jdouble>) {
				return jni->CallStaticDoubleMethodA(nullptr, m_method, arg_data);
			}
			if constexpr (std::is_same_v<R, jobject>) {
				return jni->CallStaticObjectMethodA(nullptr, m_method, arg_data);
			}
			throw std::exception("Unsupported return type");
        }
    };
}
