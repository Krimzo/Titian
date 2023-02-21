#include "web/web.h"

#include "utility/utility.h"


std::string kl::web::download_website(const std::string& url, const int buffer_size)
{
    const HINTERNET connection = InternetOpenA("Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    if (warning_check(!connection, "Failed to open a browser")) {
        return {};
    }

    const HINTERNET address = InternetOpenUrlA(connection, url.c_str(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    if (warning_check(!address, format("Failed to open url \"", url, "\""))) {
        InternetCloseHandle(connection);
        return {};
    }

    std::string data = {};
    std::vector<byte> buffer(buffer_size);

    for (DWORD bytes_read = 0; InternetReadFile(address, buffer.data(), (DWORD) buffer.size(), &bytes_read) && bytes_read > 0;) {
        for (DWORD i = 0; i < bytes_read; i++) {
            data.push_back((char) buffer[i]);
        }
    }

    InternetCloseHandle(address);
    InternetCloseHandle(connection);

    return data;
}
