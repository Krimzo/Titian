#pragma once

#include "ots/container/data_container.h"


namespace kl::ots {
	class LiteralContainer : public DataContainer
	{
		std::any m_value;

	public:
		LiteralContainer();
		LiteralContainer(const std::string& source);

		bool from_string(std::string data, Preprocessor preprocessor = {}) override;
		std::string to_string() const override;

		void put_null() override;

		void put_bool(bool value) override;
		std::optional<bool> get_bool() const override;

		void put_int(int64_t value) override;
		std::optional<int64_t> get_long() const override;
		std::optional<int32_t> get_int() const override;
		std::optional<int16_t> get_short() const override;

		void put_float(double value) override;
		std::optional<double> get_double() const override;
		std::optional<float> get_float() const override;

		void put_char(char value) override;
		std::optional<char> get_char() const override;

		void put_string(const std::string& value) override;
		std::optional<std::string> get_string() const override;

		// Helper
		static inline Object<LiteralContainer> load_null()
		{
			Object<LiteralContainer> container = new LiteralContainer();
			container->put_null();
			return container;
		}

		static inline Object<LiteralContainer> load_bool(bool value)
		{
			Object<LiteralContainer> container = new LiteralContainer();
			container->put_bool(value);
			return container;
		}

		static inline Object<LiteralContainer> load_int(int64_t value)
		{
			Object<LiteralContainer> container = new LiteralContainer();
			container->put_int(value);
			return container;
		}

		static inline Object<LiteralContainer> load_float(double value)
		{
			Object<LiteralContainer> container = new LiteralContainer();
			container->put_float(value);
			return container;
		}

		static inline Object<LiteralContainer> load_char(char value)
		{
			Object<LiteralContainer> container = new LiteralContainer();
			container->put_char(value);
			return container;
		}

		static inline Object<LiteralContainer> load_string(const std::string& value)
		{
			Object<LiteralContainer> container = new LiteralContainer();
			container->put_string(value);
			return container;
		}

	private:
		template<typename T>
		std::optional<T> get() const
		{
			if (m_value.type() == typeid(T)) {
				return { std::any_cast<T>(m_value) };
			}
			return {};
		}
	};
}
