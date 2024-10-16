#include "titian.h"


titian::BinarySerializer::BinarySerializer(const StringView& path, const bool write)
	: m_path(path), m_writing(write)
{
	m_file.open(path, write);
	if (!m_file) {
		return;
	}

	if (write) {
		write_string("serial_version", SERIAL_VERSION_STR);
		m_is_valid = true;
		Logger::log("Opened BINARY serialization file [", path, "] (", SERIAL_VERSION_STR, ")");
	}
	else {
		String version;
		read_string("serial_version", version);
		if (version == SERIAL_VERSION_STR) {
			m_is_valid = true;
			Logger::log("Opened BINARY deserialization file [", path, "] (", SERIAL_VERSION_STR, ")");
		}
		else {
			Logger::log("Failed to verify [", path, "] found serial version (", version, "), expected version (", SERIAL_VERSION_STR, ")");
		}
	}
}

titian::BinarySerializer::~BinarySerializer()
{
	if (!*this)
		return;
	Logger::log("Closed BINARY ", m_writing ? "serialization" : "deserialization", " file [", m_path, "]");
}

titian::BinarySerializer::operator bool() const
{
	return m_file && m_is_valid;
}

void titian::BinarySerializer::push_object(const StringView& name)
{}

void titian::BinarySerializer::pop_object()
{}

void titian::BinarySerializer::load_object(const StringView& name) const
{}

void titian::BinarySerializer::unload_object() const
{}

void titian::BinarySerializer::write_bool(const StringView& name, bool value)
{
	m_file.write<bool>(value);
}

void titian::BinarySerializer::read_bool(const StringView& name, bool& value) const
{
	m_file.read<bool>(value);
}

void titian::BinarySerializer::write_int(const StringView& name, int32_t value)
{
	m_file.write<int32_t>(value);
}

void titian::BinarySerializer::read_int(const StringView& name, int32_t& value) const
{
	m_file.read<int32_t>(value);
}

void titian::BinarySerializer::write_float(const StringView& name, float value)
{
	m_file.write<float>(value);
}

void titian::BinarySerializer::read_float(const StringView& name, float& value) const
{
	m_file.read<float>(value);
}

void titian::BinarySerializer::write_byte_array(const StringView& name, const void* data, int32_t count)
{
	m_file.write((const byte*) data, count);
}

void titian::BinarySerializer::read_byte_array(const StringView& name, void* data, int32_t count) const
{
	m_file.read((byte*) data, count);
}

void titian::BinarySerializer::write_int_array(const StringView& name, const int32_t* data, int32_t count)
{
	write_byte_array(name, data, count * sizeof(int32_t));
}

void titian::BinarySerializer::read_int_array(const StringView& name, int32_t* data, int32_t count) const
{
	read_byte_array(name, data, count * sizeof(int32_t));
}

void titian::BinarySerializer::write_float_array(const StringView& name, const float* data, int32_t count)
{
	write_byte_array(name, data, count * sizeof(float));
}

void titian::BinarySerializer::read_float_array(const StringView& name, float* data, int32_t count) const
{
	read_byte_array(name, data, count * sizeof(float));
}

void titian::BinarySerializer::write_string(const StringView& name, const StringView& value)
{
	write_int({}, (int32_t) value.size());
	write_byte_array({}, value.data(), (int32_t) value.size());
}

void titian::BinarySerializer::read_string(const StringView& name, String& value) const
{
	int32_t size = 0;
	read_int({}, size);
	value.resize(size);
	read_byte_array({}, value.data(), (int32_t) value.size());
}
