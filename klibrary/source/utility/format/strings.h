#pragma once

#include "apis/apis.h"


namespace kl {
    struct string_hash : std::hash<std::string_view>
    {
        using is_transparent = void;
    };
}

namespace kl {
    std::string convert_string(const std::wstring_view& data);
    std::wstring convert_string(const std::string_view& data);

    std::vector<std::string> split_string(const std::string_view& data, char delimiter);
    std::vector<std::wstring> split_string(const std::wstring_view& data, wchar_t delimiter);

    void replace_all(std::string& str, const std::string_view& from, const std::string_view& to);
    void replace_all(std::wstring& str, const std::wstring_view& from, const std::wstring_view& to);

    std::optional<int64_t> parse_int(const std::string_view& data);
    std::optional<double> parse_float(const std::string_view& data);
}

namespace kl {
    template<int W, int H, typename T>
    constexpr std::string format_matrix(const T* data)
    {
        std::string output_data[W * H] = {};
        int max_lengths[W] = {};

        for (int x = 0; x < W; x++) {
            for (int y = 0; y < H; y++) {
                output_data[y * W + x] = std::format("{:.2f}", data[y * W + x]);
                max_lengths[x] = max(max_lengths[x], (int) output_data[y * W + x].size());
            }
        }

        std::stringstream stream;
        for (int y = 0; y < H; y++) {
            stream << ((y == 0) ? (char) 218 : (y == (H - 1) ? (char) 192 : (char) 179));
            for (int x = 0; x < (W - 1); x++) {
                stream << std::setw(max_lengths[x]) << output_data[y * W + x] << " ";
            }
            stream << std::setw(max_lengths[W - 1]) << output_data[y * W + (W - 1)];
            stream << (y == 0 ? (char) 191 : (y == (H - 1) ? (char) 217 : (char) 179));
            stream << (y != (H - 1) ? "\n" : "");
        }
        return stream.str();
    }
}

namespace kl {
    template <bool NewLine = true, typename... Args>
    void write(std::ostream& stream, const Args&... args)
    {
        std::osyncstream synced_stream(stream);
        (synced_stream << ... << args);
        if constexpr (NewLine) {
            synced_stream << '\n';
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
        std::stringstream stream;
        write<false>(stream, args...);
        return stream.str();
    }

    template <bool NewLine = true, typename... Args>
    void wwrite(std::wostream& w_stream, const Args&... args)
    {
        std::wosyncstream w_synced_stream(w_stream);
        (w_synced_stream << ... << args);
        if constexpr (NewLine) {
            w_synced_stream << '\n';
        }
    }

    template <bool NewLine = true, typename... Args>
    void wprint(const Args&... args)
    {
        wwrite<NewLine>(std::wcout, args...);
    }

    template <typename... Args>
    std::wstring wformat(const Args&... args)
    {
        std::wstringstream w_stream;
        wwrite<false>(w_stream, args...);
        return w_stream.str();
    }
}
