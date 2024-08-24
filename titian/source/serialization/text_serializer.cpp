#include "titian.h"


titian::TextSerializer::TextSerializer(const StringView& path, const bool write)
	: m_path(path), m_writing(write)
{
	if (write) {
		current()["version"] = js::make_string(SERIAL_VERSION_STR);
		m_is_valid = true;
		Logger::log("Opened TEXT serialization file [", path, "]", " (", SERIAL_VERSION_STR, ")");
	}
	else {
		current() = { kl::read_file(path) };

		String version;
		if (current().contains("version")) {
			version = current().at("version")->get_string().value_or(version);
		}

		if (version == SERIAL_VERSION_STR) {
			m_is_valid = true;
			Logger::log("Opened TEXT deserialization file [", path, "]", " (", SERIAL_VERSION_STR, ")");
		}
		else {
			Logger::log("Failed to verify [", path, "] serial version (", version, "), expected version (", SERIAL_VERSION_STR, ")");
		}
	}
}

titian::TextSerializer::~TextSerializer()
{
	if (*this) {
		if (m_writing) {
			kl::write_file(m_path, m_container.decompile());
		}
		Logger::log("Closed TEXT ", m_writing ? "serialization" : "deserialization", " file [", m_path, "]");
	}
}

titian::TextSerializer::operator bool() const
{
	return m_is_valid;
}

// top object
void titian::TextSerializer::push_object(const StringView& name)
{
	auto it = current().emplace(name, kl::Wrap<js::Object>::make()).first;
	js::Object* ptr = it->second.as<js::Object>();
	m_stack.push_back(ptr);
}

void titian::TextSerializer::pop_object()
{
	if (m_stack.size() > 1) {
		m_stack.pop_back();
	}
}

void titian::TextSerializer::load_object(const StringView& name) const
{
	auto it = current().find(name);
	if (it != current().end()) {
		js::Object* ptr = (js::Object*) it->second.as<js::Object>();
		m_stack.push_back(ptr);
	}
}

void titian::TextSerializer::unload_object() const
{
	if (m_stack.size() > 1) {
		m_stack.pop_back();
	}
}

// basic
void titian::TextSerializer::write_bool(const StringView& name, bool value)
{
	current().emplace(name, js::make_bool(value));
}

void titian::TextSerializer::read_bool(const StringView& name, bool& value) const
{
	auto it = current().find(name);
	if (it != current().end()) {
		value = it->second->get_bool().value_or(value);
	}
}

void titian::TextSerializer::write_int(const StringView& name, int32_t value)
{
	current().emplace(name, js::make_number(value));
}

void titian::TextSerializer::read_int(const StringView& name, int32_t& value) const
{
	auto it = current().find(name);
	if (it != current().end()) {
		value = it->second->get_int().value_or(value);
	}
}

void titian::TextSerializer::write_float(const StringView& name, float value)
{
	current().emplace(name, js::make_number(value));
}

void titian::TextSerializer::read_float(const StringView& name, float& value) const
{
	auto it = current().find(name);
	if (it != current().end()) {
		value = it->second->get_float().value_or(value);
	}
}

// array
void titian::TextSerializer::write_byte_array(const StringView& name, const void* data, int32_t count)
{
	const byte* ptr = (const byte*) data;
	kl::Wrap result = kl::Wrap<js::Array>::make();
	for (int32_t i = 0; i < count; i++) {
		result->push_back(js::make_number(ptr[i]));
	}
	current().emplace(name, std::move(result));
}

void titian::TextSerializer::read_byte_array(const StringView& name, void* data, int32_t count) const
{
	auto it = current().find(name);
	if (it == current().end()) {
		return;
	}
	byte* ptr = (byte*) data;
	if (const js::Array* container = it->second.as<js::Array>()) {
		const int32_t min_count = kl::min((int32_t) container->size(), count);
		for (int32_t i = 0; i < min_count; i++) {
			ptr[i] = container->at(i)->get_int().value_or(0);
		}
	}
}

void titian::TextSerializer::write_int_array(const StringView& name, const int32_t* data, int32_t count)
{
	kl::Wrap result = kl::Wrap<js::Array>::make();
	for (int32_t i = 0; i < count; i++) {
		result->push_back(js::make_number(data[i]));
	}
	current().emplace(name, std::move(result));
}

void titian::TextSerializer::read_int_array(const StringView& name, int32_t* data, int32_t count) const
{
	auto it = current().find(name);
	if (it == current().end()) {
		return;
	}
	if (const js::Array* container = it->second.as<js::Array>()) {
		const int32_t min_count = kl::min((int32_t) container->size(), count);
		for (int32_t i = 0; i < min_count; i++) {
			data[i] = container->at(i)->get_int().value_or(0);
		}
	}
}

void titian::TextSerializer::write_float_array(const StringView& name, const float* data, int32_t count)
{
	kl::Wrap result = kl::Wrap<js::Array>::make();
	for (int32_t i = 0; i < count; i++) {
		result->push_back(js::make_number(data[i]));
	}
	current().emplace(name, std::move(result));
}

void titian::TextSerializer::read_float_array(const StringView& name, float* data, int32_t count) const
{
	auto it = current().find(name);
	if (it == current().end()) {
		return;
	}
	if (const js::Array* container = it->second.as<js::Array>()) {
		const int32_t min_count = kl::min((int32_t) container->size(), count);
		for (int32_t i = 0; i < min_count; i++) {
			data[i] = container->at(i)->get_float().value_or(0.0f);
		}
	}
}

// complex
void titian::TextSerializer::write_string(const StringView& name, const StringView& value)
{
	current().emplace(name, js::make_string(value));
}

void titian::TextSerializer::read_string(const StringView& name, String& value) const
{
	auto it = current().find(name);
	if (it != current().end()) {
		value = it->second->get_string().value_or(value);
	}
}
