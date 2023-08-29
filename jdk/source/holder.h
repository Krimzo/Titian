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
            if (m_object) {
                delete_object(m_object);
                m_object = nullptr;
            }
        }

        // Copy
        holder(const holder&) = delete;
        holder(const holder&&) = delete;

        void operator=(const holder&) = delete;
        void operator=(const holder&&) = delete;

        // Info
        void set(T object)
        {
            if (m_object) {
                delete_object(m_object);
            }
            m_object = object;
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
