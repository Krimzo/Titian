export module class_info;

export import java;
export import <stdarg.h>;

export namespace java {
	class ClassInfo
	{
		jclass m_class = nullptr;
		std::unordered_map<uint32_t, jmethod> m_constructors = {};
		std::unordered_map<std::string, jmethod> m_methods = {};
		std::unordered_map<std::string, jfield> m_fields = {};

	public:
		ClassInfo()
		{}

		// Load
		void load_class(const std::string& name)
		{
			const jclass clazz = java::load_eternal_class(name);
			if (!clazz) throw std::exception("Failed to load class");
			m_class = clazz;
		}

		void load_constructor(const uint32_t index, const std::vector<std::string>& parameters)
		{
			const jmethod constructor = java::get_constructor(m_class, parameters);
			if (!constructor) throw std::exception("Failed to load constructor");
			m_constructors[index] = constructor;
		}

		void load_method(const std::string& name, const std::string& return_type, const std::vector<std::string>& parameters)
		{
			const jmethod method = java::get_method(m_class, name, return_type, parameters);
			if (!method) throw std::exception("Failed to load method");
			m_methods[name] = method;
		}

		void load_field(const std::string& name, const std::string& signature)
		{
			const jfield field = java::jni->GetFieldID(m_class, name.c_str(), signature.c_str());
			if (!field) throw std::exception("Failed to load field");
			m_fields[name] = field;
		}

		// Get
		jclass get_class() const
		{
			return m_class;
		}

		jmethod get_constructor(const uint32_t index) const
		{
			return m_constructors.at(index);
		}

		jmethod get_method(const std::string& name) const
		{
			return m_methods.at(name);
		}

		jfield get_field(const std::string& name) const
		{
			return m_fields.at(name);
		}

		// Make
		jobject new_instance(const uint32_t constructor_index, ...) const
		{
			va_list args;
			va_start(args, constructor_index);
			const jobject result = java::jni->NewObjectV(m_class, m_constructors.at(constructor_index), args);
			va_end(args);
			return result;
		}
	};
}
