#pragma once

#include "application/logger.h"


namespace titian {
	std::string format_serial_version(uint32_t version);

	inline constexpr uint32_t SERIAL_VERSION = 0x09'06'2024;
	inline const std::string SERIAL_VERSION_FORMAT = format_serial_version(SERIAL_VERSION);
}

namespace titian {
	class Serializer
	{
	public:
		Serializer(const std::string& path, bool write);
		~Serializer();

		operator bool() const;

		// Single object
		template<typename T>
		void write_object(const T& object)
		{
			m_file.write<T>(object);
		}

		template<typename T>
		void read_object(T& object) const
		{
			m_file.read<T>(object);
		}

		template<typename T>
		T read_object() const
		{
			return m_file.read<T>();
		}

		// Array
		template<typename T>
		void write_array(const T* data, const uint64_t count)
		{
			m_file.write<T>(data, count);
		}

		template<typename T>
		void read_array(T* data, const uint64_t count) const
		{
			m_file.read<T>(data, count);
		}

		// String
		void write_string(const std::string& data);
		void read_string(std::string& data) const;
		std::string read_string() const;

	private:
		const std::string m_path;
		const bool m_writing;

		kl::File m_file = {};
		bool m_is_valid_version = true;
	};
}
