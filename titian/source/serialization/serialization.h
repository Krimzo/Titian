#pragma once

#include "application/logger.h"


namespace titian {
	inline String format_serial_version(const int32_t version)
	{
		String result = kl::format(std::hex, std::setfill('0'), std::setw(8), version);
		result.insert(result.begin() + 2, '\'');
		result.insert(result.begin() + 5, '\'');
		return result;
	}

	inline constexpr int32_t SERIAL_VERSION = 0x10'08'2024;
	inline const String SERIAL_VERSION_FORMAT = format_serial_version(SERIAL_VERSION);
}

namespace titian {
	class Serializer : kl::NoCopy
	{
	public:
		Serializer() = default;
		virtual ~Serializer() = default;

		virtual operator bool() const = 0;

		// object
		virtual void push_object(const String& name) = 0;
		virtual void pop_object() = 0;

		virtual void load_object(const String& name) const = 0;
		virtual void unload_object() const = 0;

		// basic
		virtual void write_bool(const String& name, bool value) = 0;
		virtual void read_bool(const String& name, bool& value) const = 0;

		virtual void write_int(const String& name, int32_t value) = 0;
		virtual void read_int(const String& name, int32_t& value) const = 0;

		virtual void write_float(const String& name, float value) = 0;
		virtual void read_float(const String& name, float& value) const = 0;

		// array
		virtual void write_byte_array(const String& name, const void* data, int32_t count) = 0;
		virtual void read_byte_array(const String& name, void* data, int32_t count) const = 0;

		virtual void write_int_array(const String& name, const int32_t* data, int32_t count) = 0;
		virtual void read_int_array(const String& name, int32_t* data, int32_t count) const = 0;

		virtual void write_float_array(const String& name, const float* data, int32_t count) = 0;
		virtual void read_float_array(const String& name, float* data, int32_t count) const = 0;

		// complex
		virtual void write_string(const String& name, const String& value) = 0;
		virtual void read_string(const String& name, String& value) const = 0;
	};
}

namespace titian {
	class Serializable
	{
	public:
		Serializable() = default;
		virtual ~Serializable() = default;

		virtual void serialize(Serializer* serializer, const void* helper_data) const = 0;
		virtual void deserialize(const Serializer* serializer, const void* helper_data) = 0;
	};
}
