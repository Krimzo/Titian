#pragma once

#include "java.h"


namespace java {
    template<typename T>
    class holder
    {
        T m_object = nullptr;

    public:
        // Construct
        holder(T object)
            : m_object(object)
        {}

        ~holder()
        {
            delete_object(m_object);
        }

        // Copy
        holder(const holder&) = delete;
        holder(const holder&&) = delete;

        void operator=(const holder&) = delete;
        void operator=(const holder&&) = delete;

        // Info
        bool set(T object)
        {
            if (m_object) {
                return false;
            }
            m_object = object;
            return true;
        }

        T get() const
        {
            return m_object;
        }

        operator bool() const
        {
            return (bool) m_object;
        }
    };
}
