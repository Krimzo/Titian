export module serializer;

export import logger;

export namespace titian {
	inline std::string format_serial_version(const uint32_t version)
	{
		return kl::format("0x", std::hex, version);
	}

	constexpr uint32_t SERIAL_VERSION = 0x31102023;
	const std::string SERIAL_VERSION_FORMAT = format_serial_version(SERIAL_VERSION);
}

export namespace titian {
	class Serializer
	{
	public:
		Serializer(const std::string& path, const bool write)
			: m_path(path), m_writing(write)
		{
			m_file.open(path, write);
			if (!m_file) {
				return;
			}

			if (write) {
				this->write_object<uint32_t>(SERIAL_VERSION);
				Logger::log("Opened file for serialization [", path, "]", " (", SERIAL_VERSION_FORMAT, ")");
			}
			else {
				const uint32_t version = this->read_object<uint32_t>();
				if (version == SERIAL_VERSION) {
					m_is_valid_version = true;
					Logger::log("Opened file for deserialization [", path, "]", " (", SERIAL_VERSION_FORMAT, ")");
				}
				else {
					m_is_valid_version = false;
					Logger::log("Failed to verify [", path, "] serial version (", format_serial_version(version), "), expected version (", SERIAL_VERSION_FORMAT, ")");
				}
			}
		}

		~Serializer()
		{
			if (*this) {
				Logger::log("Closed ", m_writing ? "serialization" : "deserialization", " file [", m_path, "]");
			}
		}

		operator bool() const
		{
			return m_file && m_is_valid_version;
		}

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
		void write_string(const std::string& data)
		{
			const uint64_t size = data.size();
			m_file.write<uint64_t>(size);
			m_file.write<char>(data.data(), size);
		}

		void read_string(std::string& data) const
		{
			const uint64_t size = m_file.read<uint64_t>();
			data.resize(size);
			m_file.read<char>(data.data(), size);
		}

		std::string read_string() const
		{
			std::string result = {};
			read_string(result);
			return result;
		}

	private:
		const std::string m_path;
		const bool m_writing;

		kl::File m_file = {};
		bool m_is_valid_version = true;
	};
}
