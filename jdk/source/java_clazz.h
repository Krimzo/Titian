#pragma once

#include "java.h"


namespace java {
    class clazz
    {
        jobject m_loader = nullptr;
        jclass m_clazz = nullptr;

    public:
        clazz(const std::string& path);
        clazz(const clazz_data& data);
        virtual ~clazz();

        clazz(const clazz&) = delete;
        clazz(const clazz&&) = delete;

        void operator=(const clazz&) = delete;
        void operator=(const clazz&&) = delete;

        // Get
        operator jclass() const;
        operator bool() const;

        // Methods
        jmethod get_static_method(const std::string& name, const std::string& return_type = VOID, const std::vector<std::string>& parameters = {});
        jmethod get_method(const std::string& name, const std::string& return_type = VOID, const std::vector<std::string>& parameters = {});
        jmethod get_constructor(const std::vector<std::string>& parameters = {});

        // Objects
        jobject new_object(jmethod constructor, ...);
    };
}
