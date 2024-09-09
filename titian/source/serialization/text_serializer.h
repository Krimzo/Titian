#pragma once

#include "serialization/serialization.h"


namespace titian {
	struct TextSerializer : Serializer
	{
		TextSerializer(const StringView& path, bool write);
		~TextSerializer();

		operator bool() const override;

		void push_object(const StringView& name) override;
		void pop_object() override;

		void load_object(const StringView& name) const override;
		void unload_object() const override;

		void write_bool(const StringView& name, bool value) override;
		void read_bool(const StringView& name, bool& value) const override;

		void write_int(const StringView& name, int32_t value) override;
		void read_int(const StringView& name, int32_t& value) const override;

		void write_float(const StringView& name, float value) override;
		void read_float(const StringView& name, float& value) const override;

		void write_byte_array(const StringView& name, const void* data, int32_t count) override;
		void read_byte_array(const StringView& name, void* data, int32_t count) const override;

		void write_int_array(const StringView& name, const int32_t* data, int32_t count) override;
		void read_int_array(const StringView& name, int32_t* data, int32_t count) const override;

		void write_float_array(const StringView& name, const float* data, int32_t count) override;
		void read_float_array(const StringView& name, float* data, int32_t count) const override;

		void write_string(const StringView& name, const StringView& value) override;
		void read_string(const StringView& name, String& value) const override;

	private:
		const String m_path;
		const bool m_writing;

		js::Object m_container;
		mutable Vector<js::Object*> m_stack = { &m_container };
		bool m_is_valid = false;

		auto& current() const
		{
			return *m_stack.back();
		}
	};
}
