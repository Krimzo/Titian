#pragma once

#include "json/container/container.h"


namespace kl::json {
	class Literal : public Container
	{
	public:
		Literal();
		Literal(const std::string& data);

		bool compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last) override;
		std::string decompile(int depth = 0) const override;

		void put_null() override;

		void put_bool(bool value) override;
		std::optional<bool> get_bool() const override;

		void put_number(double value) override;
		std::optional<double> get_double() const override;
		std::optional<float> get_float() const override;
		std::optional<int64_t> get_long() const override;
		std::optional<int32_t> get_int() const override;
		std::optional<int16_t> get_short() const override;
		std::optional<uint8_t> get_byte() const override;

		void put_string(const std::string& value) override;
		std::optional<std::string> get_string() const override;

	private:
		std::variant<std::nullptr_t, bool, double, std::string> m_value;

		template<typename T>
		std::optional<T> try_get() const
		{
			if (const T* ptr = std::get_if<T>(&m_value)) {
				return std::optional<T>{ *ptr };
			}
			return std::nullopt;
		}
	};
}

namespace kl::json {
	static inline Wrap<Literal> make_null()
	{
		Wrap result = Wrap<Literal>::make();
		result->put_null();
		return result;
	}

	static inline Wrap<Literal> make_bool(bool value)
	{
		Wrap result = Wrap<Literal>::make();
		result->put_bool(value);
		return result;
	}

	static inline Wrap<Literal> make_number(double value)
	{
		Wrap result = Wrap<Literal>::make();
		result->put_number(value);
		return result;
	}

	static inline Wrap<Literal> make_string(const std::string& value)
	{
		Wrap result = Wrap<Literal>::make();
		result->put_string(value);
		return result;
	}
}
