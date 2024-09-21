#pragma once

#include "apis/apis.h"

#define DLL_EXPORT(return_type) extern "C" return_type __declspec(dllexport) __stdcall


namespace kl {
    struct DLL : NoCopy
    {
        template<typename Return, typename... Args>
        using Function = Return(__stdcall*)(Args...);

        DLL();
        DLL(const std::string_view& path);
        ~DLL();

        operator bool() const;

        void load(const std::string_view& path);
        void reload();
        void unload();

        template<typename Return, typename... Args>
        Function<Return, Args...> read_function(const std::string_view& function_name)
        {
            if (!m_module) {
                return nullptr;
            }
            auto function_address = GetProcAddress(m_module, function_name.data());
            return (Function<Return, Args...>) function_address;
        }

    private:
        std::string m_path;
        HMODULE m_module = nullptr;
    };
}
