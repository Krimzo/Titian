#include "titian.h"


titian::BinarySerializer::BinarySerializer(const String& path, const bool write)
	: m_path(path), m_writing(write)
{
	m_file.open(path, write);
	if (!m_file) {
		return;
	}

	if (write) {
		write_int({}, SERIAL_VERSION);
		m_is_valid = true;
		Logger::log("Opened BINARY serialization file [", path, "]", " (", SERIAL_VERSION_FORMAT, ")");
	}
	else {
		int32_t version = 0;
		read_int({}, version);
		if (version == SERIAL_VERSION) {
			m_is_valid = true;
			Logger::log("Opened BINARY deserialization file [", path, "]", " (", SERIAL_VERSION_FORMAT, ")");
		}
		else {
			Logger::log("Failed to verify [", path, "] found serial version (", format_serial_version(version), "), expected version (", SERIAL_VERSION_FORMAT, ")");
		}
	}
}

titian::BinarySerializer::~BinarySerializer()
{
	if (*this) {
		Logger::log("Closed binary ", m_writing ? "serialization" : "deserialization", " file [", m_path, "]");
	}
}

titian::BinarySerializer::operator bool() const
{
	return m_file && m_is_valid;
}

void titian::BinarySerializer::push_object(const String& name)
{}

void titian::BinarySerializer::pop_object()
{}

void titian::BinarySerializer::load_object(const String& name) const
{}

void titian::BinarySerializer::unload_object() const
{}

// basic
void titian::BinarySerializer::write_bool(const String& name, bool value)
{
	m_file.write<bool>(value);
}

void titian::BinarySerializer::read_bool(const String& name, bool& value) const
{
	m_file.read<bool>(value);
}

void titian::BinarySerializer::write_int(const String& name, int32_t value)
{
	m_file.write<int32_t>(value);
}

void titian::BinarySerializer::read_int(const String& name, int32_t& value) const
{
	m_file.read<int32_t>(value);
}

void titian::BinarySerializer::write_float(const String& name, float value)
{
	m_file.write<float>(value);
}

void titian::BinarySerializer::read_float(const String& name, float& value) const
{
	m_file.read<float>(value);
}

// array
void titian::BinarySerializer::write_byte_array(const String& name, const void* data, int32_t count)
{
	m_file.write((const byte*) data, count);
}

void titian::BinarySerializer::read_byte_array(const String& name, void* data, int32_t count) const
{
	m_file.read((byte*) data, count);
}

void titian::BinarySerializer::write_int_array(const String& name, const int32_t* data, int32_t count)
{
	write_byte_array(name, data, count * sizeof(int32_t));
}

void titian::BinarySerializer::read_int_array(const String& name, int32_t* data, int32_t count) const
{
	read_byte_array(name, data, count * sizeof(int32_t));
}

void titian::BinarySerializer::write_float_array(const String& name, const float* data, int32_t count)
{
	write_byte_array(name, data, count * sizeof(float));
}

void titian::BinarySerializer::read_float_array(const String& name, float* data, int32_t count) const
{
	read_byte_array(name, data, count * sizeof(float));
}

// complex
void titian::BinarySerializer::write_string(const String& name, const String& value)
{
	write_int({}, (int32_t) value.size());
	write_byte_array({}, value.data(), (int32_t) value.size());
}

void titian::BinarySerializer::read_string(const String& name, String& value) const
{
	int32_t size = 0;
	read_int({}, size);
	value.resize(size);
	read_byte_array({}, value.data(), (int32_t) value.size());
}
