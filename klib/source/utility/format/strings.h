#pragma once

#include "apis/apis.h"


namespace kl::strings {
    std::string to_string(const std::wstring& data);
    std::wstring to_w_string(const std::string& data);

    std::vector<std::string> split(const std::string& data, char delimiter);
    std::vector<std::wstring> w_split(const std::wstring& data, wchar_t delimiter);
}

namespace kl {
    template<int W, int H>
    std::string format_matrix(const float* data)
    {
        std::string output_data[W * H] = {};
        int max_lengths[W] = {};

        for (int x = 0; x < W; x++) {
            for (int y = 0; y < H; y++) {
                output_data[y * W + x] = std::format("{:.2f}", data[y * W + x]);
                max_lengths[x] = max(max_lengths[x], (int) output_data[y * W + x].size());
            }
        }

        std::stringstream stream = {};
        for (int y = 0; y < H; y++) {
            stream << ((y == 0) ? char(218) : (y == (H - 1) ? char(192) : char(179)));
            for (int x = 0; x < (W - 1); x++) {
                stream << std::setw(max_lengths[x]) << output_data[y * W + x] << " ";
            }
            stream << std::setw(max_lengths[W - 1]) << output_data[y * W + (W - 1)];
            stream << (y == 0 ? char(191) : (y == (H - 1) ? char(217) : char(179)));
            stream << (y != (H - 1) ? "\n" : "");
        }
        return stream.str();
    }
}

namespace kl {
    // 8 bit chars
    template <bool NewLine = true, typename... Args>
    void write(std::ostream& stream, const Args&... args)
    {
        std::osyncstream synced_stream(stream);
        (synced_stream << ... << args);
        if constexpr (NewLine) {
            synced_stream << std::endl;
        }
    }

    template <bool NewLine = true, typename... Args>
    void print(const Args&... args)
    {
        write<NewLine>(std::cout, args...);
    }

    template <typename... Args>
    std::string format(const Args&... args)
    {
        std::stringstream stream = {};
        write<false>(stream, args...);
        return stream.str();
    }

    // 16 bit chars
    template <bool NewLine = true, typename... Args>
    void w_write(std::wostream& w_stream, const Args&... args)
    {
        std::wosyncstream w_synced_stream(w_stream);
        (w_synced_stream << ... << args);
        if constexpr (NewLine) {
            w_synced_stream << std::endl;
        }
    }

    template <bool NewLine = true, typename... Args>
    void w_print(const Args&... args)
    {
        w_write<NewLine>(std::wcout, args...);
    }

    template <typename... Args>
    std::wstring w_format(const Args&... args)
    {
        std::wstringstream w_stream;
        w_write<false>(w_stream, args...);
        return w_stream.str();
    }
}
