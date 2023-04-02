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
        std::string path_ = "";
        HMODULE module_ = nullptr;

    public:
        dll();
        dll(const std::string& path);
        ~dll();

        dll(const dll&) = delete;
        dll(const dll&&) = delete;

        void operator=(const dll&) = delete;
        void operator=(const dll&&) = delete;

        operator bool() const;

        void load(const std::string& path);
        void reload();
        void unload();

        template<typename Return, typename... Args>
        function<Return, Args...> get_function(const std::string& function_name)
        {
            if (!module_) {
                return nullptr;
            }
            auto function_address = GetProcAddress(module_, function_name.c_str());
            return (function<Return, Args...>) function_address;
        }
    };
}
