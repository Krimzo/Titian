#pragma once

#include "json/container/container.h"


namespace kl::json {
	class Literal : public Container
	{
		std::any m_value;

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

		// Helper
		static inline Ref<Literal> make_null()
		{
			Ref container = new Literal();
			container->put_null();
			return container;
		}

		static inline Ref<Literal> make_bool(bool value)
		{
			Ref container = new Literal();
			container->put_bool(value);
			return container;
		}

		static inline Ref<Literal> make_number(double value)
		{
			Ref container = new Literal();
			container->put_number(value);
			return container;
		}

		static inline Ref<Literal> make_string(const std::string& value)
		{
			Ref container = new Literal();
			container->put_string(value);
			return container;
		}

	private:
		template<typename T>
		std::optional<T> try_get() const
		{
			if (m_value.type() == typeid(T)) {
				return { std::any_cast<T>(m_value) };
			}
			return {};
		}
	};
}
