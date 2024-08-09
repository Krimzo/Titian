#pragma once

#include "serialization/serialization.h"


namespace titian {
	class TextSerializer : public Serializer
	{
	public:
		TextSerializer(const String& path, bool write);
		~TextSerializer();

		operator bool() const override;

		// top object
		void push_object(const String& name) override;
		void pop_object() override;

		void load_object(const String& name) const override;
		void unload_object() const override;

		// basic
		void write_bool(const String& name, bool value) override;
		void read_bool(const String& name, bool& value) const override;

		void write_int(const String& name, int32_t value) override;
		void read_int(const String& name, int32_t& value) const override;

		void write_float(const String& name, float value) override;
		void read_float(const String& name, float& value) const override;

		// array
		void write_byte_array(const String& name, const void* data, int32_t count) override;
		void read_byte_array(const String& name, void* data, int32_t count) const override;

		void write_int_array(const String& name, const int32_t* data, int32_t count) override;
		void read_int_array(const String& name, int32_t* data, int32_t count) const override;

		void write_float_array(const String& name, const float* data, int32_t count) override;
		void read_float_array(const String& name, float* data, int32_t count) const override;

		// complex
		void write_string(const String& name, const String& value) override;
		void read_string(const String& name, String& value) const override;

	private:
		const String m_path;
		const bool m_writing;

		js::Object m_container;
		mutable Vector<js::Object*> m_stack = { &m_container };
		bool m_is_valid = false;

		inline js::Object& current()
		{
			return *m_stack.back();
		}

		inline const js::Object& current() const
		{
			return *m_stack.back();
		}
	};
}
