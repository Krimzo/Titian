export module serializer;

export import klib;

export namespace titian {
	class Serializer
	{
	public:
		Serializer(const std::string_view& path, const bool write)
		{
			m_file.open(path, write);
		}

		operator bool() const
		{
			return static_cast<bool>(m_file);
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
		kl::File m_file = {};
	};
}
