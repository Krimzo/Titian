#include "klibrary.h"


std::string kl::convert_string(const std::wstring& data)
{
    std::string temp;
    temp.reserve(data.size());
    for (const wchar_t c : data) {
        if (c < 256) {
            temp.push_back((char) c);
        }
    }
    return temp;
}

std::wstring kl::convert_string(const std::string& data)
{
    std::wstring temp;
    temp.resize(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        temp[i] = data[i];
    }
    return temp;
}

std::vector<std::string> kl::split_string(const std::string& data, const char delimiter)
{
    std::stringstream stream{ data };
    std::vector<std::string> parts{};
    for (std::string part; std::getline(stream, part, delimiter);) {
        parts.push_back(part);
    }
    return parts;
}

std::vector<std::wstring> kl::split_string(const std::wstring& data, const wchar_t delimiter)
{
    std::wstringstream stream{ data };
    std::vector<std::wstring> parts;
    for (std::wstring part; std::getline(stream, part, delimiter);) {
        parts.push_back(part);
    }
    return parts;
}

void kl::replace_all(std::string& str, const std::string_view& from, const std::string_view& to)
{
    if (str.empty() || from.empty())
        return;

    size_t index = 0;
    while ((index = str.find(from, index)) != -1) {
        str.replace(index, from.length(), to);
        index += to.length();
    }
}

void kl::replace_all(std::wstring& str, const std::wstring_view& from, const std::wstring_view& to)
{
    if (str.empty() || from.empty())
        return;

    size_t index = 0;
    while ((index = str.find(from, index)) != -1) {
        str.replace(index, from.length(), to);
        index += to.length();
    }
}

std::optional<int64_t> kl::parse_int(const std::string_view& data)
{
    if (data.empty()) {
        return std::nullopt;
    }
    char* last_char = nullptr;
    int64_t result = std::strtoll(data.data(), &last_char, 10);
    if (last_char != data.data() + data.size()) {
        return std::nullopt;
    }
    return { result };
}

std::optional<double> kl::parse_float(const std::string_view& data)
{
    if (data.empty()) {
        return std::nullopt;
    }
    char* last_char = nullptr;
    double result = std::strtod(data.data(), &last_char);
    if (last_char != data.data() + data.size()) {
        return std::nullopt;
    }
    return { result };
}
