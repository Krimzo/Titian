#include "klibrary.h"


kl::json::Literal::Literal()
{}

kl::json::Literal::Literal(const std::string_view& data)
{
    const auto tokens = Lexer::parse(data);
	compile(tokens.begin(), tokens.end());
}

bool kl::json::Literal::compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last)
{
    if (first == last)
        return false;

    switch (first->type)
    {
    case TokenType::VAL_NULL:
		put_null();
		return true;

	case TokenType::VAL_FALSE:
		put_bool(false);
		return true;

	case TokenType::VAL_TRUE:
		put_bool(true);
		return true;

	case TokenType::LIT_NUMBER:
		if (auto opt = parse_float(first->value)) {
			put_number(opt.value());
			return true;
		}
        break;

	case TokenType::LIT_STRING:
		put_string(first->value);
		return true;
	}
    return false;
}

std::string kl::json::Literal::decompile(const int depth) const
{
    if (auto opt = get_bool()) {
        return std::string{ opt.value() ? Standard::true_val : Standard::false_val };
    }
	if (auto opt = get_double()) {
        const double flt_value = opt.value();
        const int64_t int_value = (int64_t) flt_value;
        if (int_value == flt_value) {
            return std::to_string(int_value);
        }
        return format(flt_value);
	}
    if (auto opt = get_string()) {
        Lexer::from_escaping(opt.value());
		return format(Standard::string, opt.value(), Standard::string);
    }
    return std::string{ Standard::null_val };
}

void kl::json::Literal::put_null()
{
    m_value.emplace<std::nullptr_t>(nullptr);
}

void kl::json::Literal::put_bool(const bool value)
{
    m_value.emplace<bool>(value);
}

std::optional<bool> kl::json::Literal::get_bool() const
{
    return try_get<bool>();
}

void kl::json::Literal::put_number(const double value)
{
	m_value.emplace<double>(value);
}

std::optional<double> kl::json::Literal::get_double() const
{
    return try_get<double>();
}

std::optional<float> kl::json::Literal::get_float() const
{
    if (auto opt = get_double()) {
        return float(opt.value());
    }
    return std::nullopt;
}

std::optional<int64_t> kl::json::Literal::get_long() const
{
    if (auto opt = get_double()) {
        return int64_t(opt.value());
    }
    return std::nullopt;
}

std::optional<int32_t> kl::json::Literal::get_int() const
{
    if (auto opt = get_double()) {
        return int32_t(opt.value());
    }
    return std::nullopt;
}

std::optional<int16_t> kl::json::Literal::get_short() const
{
    if (auto opt = get_double()) {
        return int16_t(opt.value());
    }
    return std::nullopt;
}

std::optional<uint8_t> kl::json::Literal::get_byte() const
{
    if (auto opt = get_double()) {
        return uint8_t(opt.value());
    }
    return std::nullopt;
}

void kl::json::Literal::put_string(const std::string_view& value)
{
	m_value.emplace<std::string>(value);
}

std::optional<std::string> kl::json::Literal::get_string() const
{
	return try_get<std::string>();
}
