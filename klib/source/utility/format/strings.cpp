#include "utility/format/strings.h"

#include "math/math.h"


std::string kl::convert_string(const std::wstring& data)
{
    std::string temp;
    temp.reserve(data.size());
    for (const wchar_t c : data) {
        if (c < 256) {
            temp.push_back(char(c));
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
    std::vector<std::string> parts;
    std::stringstream stream(data);
    for (std::string part; std::getline(stream, part, delimiter);) {
        parts.push_back(part);
    }
    return parts;
}

std::vector<std::wstring> kl::split_string(const std::wstring& data, const wchar_t delimiter)
{
    std::vector<std::wstring> parts;
    std::wstringstream stream(data);
    for (std::wstring part; std::getline(stream, part, delimiter);) {
        parts.push_back(part);
    }
    return parts;
}
