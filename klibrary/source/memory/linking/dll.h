#pragma once

#include "apis/apis.h"

#define DLL_EXPORT(return_type) extern "C" return_type __declspec(dllexport) __stdcall


namespace kl {
    class DLL
    {
    public:
        template<typename Return, typename... Args>
        using Function = Return(__stdcall*)(Args...);

    private:
        std::string m_path = "";
        HMODULE m_module = nullptr;

    public:
        DLL();
        DLL(const std::string& path);
        virtual ~DLL();

        DLL(const DLL&) = delete;
        DLL(DLL&&) = delete;

        void operator=(const DLL&) = delete;
        void operator=(DLL&&) = delete;

        operator bool() const;

        void load(const std::string& path);
        void reload();
        void unload();

        template<typename Return, typename... Args>
        Function<Return, Args...> read_function(const std::string& function_name)
        {
            if (!m_module) {
                return nullptr;
            }
            auto function_address = GetProcAddress(m_module, function_name.c_str());
            return (Function<Return, Args...>) function_address;
        }
    };
}
