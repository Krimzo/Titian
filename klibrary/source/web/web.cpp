#include "klibrary.h"


std::string kl::download_website(const std::string_view& url, const int buffer_size)
{
    const HINTERNET connection = InternetOpenA("Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    if (!verify(connection, "Failed to open a browser")) {
        return {};
    }

    const HINTERNET address = InternetOpenUrlA(connection, url.data(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    if (!verify(address, "Failed to open url \"", url, "\"")) {
        InternetCloseHandle(connection);
        return {};
    }

    std::string data = {};
    std::vector<byte> buffer(buffer_size);
    for (DWORD bytes_read = 0; InternetReadFile(address, buffer.data(), (DWORD) buffer.size(), &bytes_read) && bytes_read > 0;) {
        data.insert(data.end(), buffer.begin(), buffer.begin() + bytes_read);
    }

    InternetCloseHandle(address);
    InternetCloseHandle(connection);
    return data;
}
