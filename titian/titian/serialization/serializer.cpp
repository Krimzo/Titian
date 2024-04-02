#include "main.h"


std::string titian::format_serial_version(const uint32_t version)
{
	return kl::format("0x", std::hex, version);
}

titian::Serializer::Serializer(const std::string& path, const bool write)
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

titian::Serializer::~Serializer()
{
	if (*this) {
		Logger::log("Closed ", m_writing ? "serialization" : "deserialization", " file [", m_path, "]");
	}
}

titian::Serializer::operator bool() const
{
	return m_file && m_is_valid_version;
}

void titian::Serializer::write_string(const std::string& data)
{
	const uint64_t size = data.size();
	m_file.write<uint64_t>(size);
	m_file.write<char>(data.data(), size);
}

void titian::Serializer::read_string(std::string& data) const
{
	const uint64_t size = m_file.read<uint64_t>();
	data.resize(size);
	m_file.read<char>(data.data(), size);
}

std::string titian::Serializer::read_string() const
{
	std::string result;
	read_string(result);
	return result;
}
