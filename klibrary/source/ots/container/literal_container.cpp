#include "klibrary.h"


kl::ots::LiteralContainer::LiteralContainer()
{}

kl::ots::LiteralContainer::LiteralContainer(const std::string& source)
{
	from_string(source);
}

bool kl::ots::LiteralContainer::from_string(std::string data, Preprocessor preprocessor)
{
    preprocessor.process(data);
    if (data.empty()) {
        return false;
    }

    // Null object
    if (data == Standard::null_value) {
        put_null();
        return true;
    }

    // Booleans
    if (data == Standard::false_value) {
		put_bool(false);
        return true;
    }
    if (data == Standard::true_value) {
		put_bool(true);
        return true;
    }

    // Chars/Strings
    if (data.size() >= 3 && data.front() == Standard::char_literal && data.back() == Standard::char_literal) {
        put_char(data[1]); // currently doesn't support multi-character chars
        return true;
    }
    if (data.size() >= 2 && data.front() == Standard::string_literal && data.back() == Standard::string_literal) {
		put_string(data.substr(1, data.size() - 2));
        return true;
    }

    // Scalars
    if (std::optional result = parse_int(data)) {
        put_int(result.value());
        return true;
    }
    if (std::optional result = parse_float(data)) {
        put_float(result.value());
        return true;
    }
    return false;
}

std::string kl::ots::LiteralContainer::to_string() const
{
    if (std::optional value = get_bool()) {
        return value.value() ? Standard::true_value : Standard::false_value;
    }
    if (std::optional value = get_long()) {
        return std::to_string(value.value());
    }
	if (std::optional value = get_double()) {
		return std::to_string(value.value());
	}
	if (std::optional value = get_char()) {
		return format(Standard::char_literal, value.value(), Standard::char_literal);
	}
    if (std::optional value = get_string()) {
		return format(Standard::string_literal, value.value(), Standard::string_literal);
    }
    return Standard::null_value;
}

void kl::ots::LiteralContainer::put_null()
{
	m_value = std::nullopt;
}

void kl::ots::LiteralContainer::put_bool(bool value)
{
    m_value = value;
}

std::optional<bool> kl::ots::LiteralContainer::get_bool() const
{
    return get<bool>();
}

void kl::ots::LiteralContainer::put_int(int64_t value)
{
	m_value = value;
}

std::optional<int64_t> kl::ots::LiteralContainer::get_long() const
{
    return get<int64_t>();
}

std::optional<int32_t> kl::ots::LiteralContainer::get_int() const
{
    if (std::optional value = get_long()) {
		return static_cast<int32_t>(value.value());
	}
    return std::nullopt;
}

std::optional<int16_t> kl::ots::LiteralContainer::get_short() const
{
	if (std::optional value = get_long()) {
		return static_cast<int16_t>(value.value());
	}
	return std::nullopt;
}

void kl::ots::LiteralContainer::put_float(double value)
{
	m_value = value;
}

std::optional<double> kl::ots::LiteralContainer::get_double() const
{
    return get<double>();
}

std::optional<float> kl::ots::LiteralContainer::get_float() const
{
	if (std::optional value = get_double()) {
		return static_cast<float>(value.value());
	}
	return std::nullopt;
}

void kl::ots::LiteralContainer::put_char(char value)
{
	m_value = value;
}

std::optional<char> kl::ots::LiteralContainer::get_char() const
{
	return get<char>();
}

void kl::ots::LiteralContainer::put_string(const std::string& value)
{
	m_value = value;
}

std::optional<std::string> kl::ots::LiteralContainer::get_string() const
{
	return get<std::string>();
}
