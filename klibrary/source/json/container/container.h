#pragma once

#include "memory/memory.h"
#include "json/language/preprocessor.h"


namespace kl::json {
	class Container
	{
	public:
		virtual ~Container() = default;

		virtual bool from_string(std::string data, Preprocessor preprocessor = {}) = 0;
		virtual std::string to_string(int depth = 0) const = 0;

		virtual void put_null() {}
		
		virtual void put_bool(bool value) {}
		virtual std::optional<bool> get_bool() const { return std::nullopt; }

		virtual void put_number(double value) {}
		virtual std::optional<double> get_double() const { return std::nullopt; }
		virtual std::optional<float> get_float() const { return std::nullopt; }
		virtual std::optional<int64_t> get_long() const { return std::nullopt; }
		virtual std::optional<int32_t> get_int() const { return std::nullopt; }
		virtual std::optional<int16_t> get_short() const { return std::nullopt; }
		virtual std::optional<uint8_t> get_byte() const { return std::nullopt; }

		virtual void put_string(const std::string& value) {}
		virtual std::optional<std::string> get_string() const { return std::nullopt; }
	};
}
