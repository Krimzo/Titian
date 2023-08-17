#pragma once

#include "java.h"


namespace java {
    class object
    {
        jobject m_object = nullptr;

    public:
        object(jobject object);
        virtual ~object();

        object(const object&) = delete;
        object(const object&&) = delete;

        void operator=(const object&) = delete;
        void operator=(const object&&) = delete;

        // Get
        operator jobject() const;
        operator bool() const;
    };
}
