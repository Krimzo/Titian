#pragma once

#include "serial/serial.h"


namespace titian {
	struct TextSerializer : Serializer
	{
		TextSerializer(const StringRef& path, bool write);
		~TextSerializer() override;

		operator bool() const override;

		void push_object(const StringRef& name) override;
		void pop_object() override;

		void load_object(const StringRef& name) const override;
		void unload_object() const override;

		void write_bool(const StringRef& name, bool value) override;
		void read_bool(const StringRef& name, bool& value) const override;

		void write_int(const StringRef& name, int32_t value) override;
		void read_int(const StringRef& name, int32_t& value) const override;

		void write_float(const StringRef& name, float value) override;
		void read_float(const StringRef& name, float& value) const override;

		void write_byte_array(const StringRef& name, const void* data, int32_t count) override;
		void read_byte_array(const StringRef& name, void* data, int32_t count) const override;

		void write_int_array(const StringRef& name, const int32_t* data, int32_t count) override;
		void read_int_array(const StringRef& name, int32_t* data, int32_t count) const override;

		void write_float_array(const StringRef& name, const float* data, int32_t count) override;
		void read_float_array(const StringRef& name, float* data, int32_t count) const override;

		void write_string(const StringRef& name, const StringRef& value) override;
		void read_string(const StringRef& name, String& value) const override;

	private:
		const String m_path;
		const bool m_writing;

		mutable Vector<Ref<js::Object>> m_stack = { new js::Object() };
		bool m_is_valid = false;

		auto& current() const
		{
			return *m_stack.back();
		}
	};
}
