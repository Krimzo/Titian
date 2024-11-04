#pragma once

#include "app/logger.h"


namespace titian {
	String format_serial_version(int32_t version);
	inline constexpr int32_t SERIAL_VERSION_INT = 0x17'10'2024;
	inline const String SERIAL_VERSION_STR = format_serial_version(SERIAL_VERSION_INT);
}

namespace titian {
	struct Serializer : kl::NoCopy
	{
		Serializer() = default;
		virtual ~Serializer() = default;

		virtual operator bool() const = 0;

		virtual void push_object(const StringRef& name) = 0;
		virtual void pop_object() = 0;

		virtual void load_object(const StringRef& name) const = 0;
		virtual void unload_object() const = 0;

		virtual void write_bool(const StringRef& name, bool value) = 0;
		virtual void read_bool(const StringRef& name, bool& value) const = 0;

		virtual void write_int(const StringRef& name, int32_t value) = 0;
		virtual void read_int(const StringRef& name, int32_t& value) const = 0;

		virtual void write_float(const StringRef& name, float value) = 0;
		virtual void read_float(const StringRef& name, float& value) const = 0;

		virtual void write_byte_array(const StringRef& name, const void* data, int32_t count) = 0;
		virtual void read_byte_array(const StringRef& name, void* data, int32_t count) const = 0;

		virtual void write_int_array(const StringRef& name, const int32_t* data, int32_t count) = 0;
		virtual void read_int_array(const StringRef& name, int32_t* data, int32_t count) const = 0;

		virtual void write_float_array(const StringRef& name, const float* data, int32_t count) = 0;
		virtual void read_float_array(const StringRef& name, float* data, int32_t count) const = 0;

		virtual void write_string(const StringRef& name, const StringRef& value) = 0;
		virtual void read_string(const StringRef& name, String& value) const = 0;
	};
}

namespace titian {
	struct Serializable
	{
		Serializable() = default;
		virtual ~Serializable() = default;

		virtual void serialize(Serializer& serializer) const = 0;
		virtual void deserialize(const Serializer& serializer) = 0;
	};
}
