#pragma once

#include "apis/apis.h"

#define DLL_EXPORT(return_type) extern "C" return_type __declspec(dllexport) __stdcall


namespace kl {
    class dll
    {
    public:
        template<typename Return, typename... Args>
        using function = Return(__stdcall*)(Args...);

    private:
        std::string path = "";
        HMODULE module = nullptr;

    public:
        dll();
        explicit dll(const std::string& path);
        ~dll();

        dll(const dll&) = delete;
        dll(const dll&&) = delete;

        void operator=(const dll&) = delete;
        void operator=(const dll&&) = delete;

        void load(const std::string& path);

        void reload();

        void unload();

        bool is_valid();

        template<typename Return, typename... Args>
        function<Return, Args...> load_function(const std::string& function_name)
        {
            return (function<Return, Args...>) (module ? GetProcAddress(module, function_name.c_str()) : nullptr);
        }
    };
}
