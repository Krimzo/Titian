#include "utility/format/strings.h"

#include "math/math.h"


std::string kl::strings::to_string(const std::wstring& data)
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

std::wstring kl::strings::to_w_string(const std::string& data)
{
    std::wstring temp;
    temp.resize(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        temp[i] = data[i];
    }
    return temp;
}

std::vector<std::string> kl::strings::split(const std::string& data, const char delimiter)
{
    std::vector<std::string> parts;
    std::stringstream stream(data);
    for (std::string part; std::getline(stream, part, delimiter);) {
        parts.push_back(part);
    }
    return parts;
}

std::vector<std::wstring> kl::strings::w_split(const std::wstring& data, const wchar_t delimiter)
{
    std::vector<std::wstring> parts;
    std::wstringstream stream(data);
    for (std::wstring part; std::getline(stream, part, delimiter);) {
        parts.push_back(part);
    }
    return parts;
}
