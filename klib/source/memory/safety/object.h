#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T>
    class Object
    {
        template<typename>
        friend class Object;

        T* m_instance = nullptr;
        std::atomic<uint64_t>* m_count = nullptr;

        uint64_t increase_count() const
        {
            return m_count ? (*m_count += 1) : 0;
        }

        uint64_t decrease_count() const
        {
            return m_count ? (*m_count -= 1) : 0;
        }

        void allocate()
        {
            m_count = new std::atomic<uint64_t>;
            if (!m_count) throw std::runtime_error("Could not allocate memory for reference counter.");
            *m_count = 1;
        }

        void deallocate() const
        {
            if (m_instance) delete m_instance;
            if (m_count) delete m_count;
        }

        void clear()
        {
            m_instance = nullptr;
            m_count = nullptr;
        }

    public:
        // Create
        Object()
        {}

        Object(T* instance)
            : m_instance(instance)
        {
            if (m_instance) {
                allocate();
            }
        }

        // Destroy
        virtual ~Object()
        {
            this->free();
        }

        void free()
        {
            if (decrease_count() == 0) {
                deallocate();
            }
            clear();
        }

        // Create copy
        Object(const Object& other)
            : m_instance(other.m_instance), m_count(other.m_count)
        {
            increase_count();
        }

        Object(const Object&& other) noexcept
            : Object(other)
        {}

        // Copy
        Object& operator=(const Object& other)
        {
            if (other.m_instance != m_instance) {
                this->free();
                m_instance = other.m_instance;
                m_count = other.m_count;
                increase_count();
            }
            return *this;
        }

        Object& operator=(Object&& other) noexcept
        {
            return (*this = other);
        }

        // Derived cast
        template<typename B> requires std::is_base_of_v<B, T>
        operator Object<B> ()
        {
            Object<B> result = {};
            result.m_instance = m_instance;
            result.m_count = m_count;
            increase_count();
            return result;
        }

        // Info
        operator bool() const
        {
            return (bool) m_instance;
        }

        uint64_t count() const
        {
            return m_count ? m_count->load() : 0;
        }

        // Compare
        template<typename O>
        bool operator==(const Object<O>& other) const
        {
            const void* first = m_instance;
            const void* second = other.m_instance;
            return (first == second);
        }

        template<typename O>
        bool operator!=(const Object<O>& other) const
        {
            return !(*this == other);
        }

        // Address
        T* operator&()
        {
            return m_instance;
        }

        const T* operator&() const
        {
            return m_instance;
        }

        // Access
        T& operator*()
        {
            return *m_instance;
        }

        const T& operator*() const
        {
            return *m_instance;
        }

        T* operator->()
        {
            return m_instance;
        }

        const T* operator->() const
        {
            return m_instance;
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Object<T>& object)
    {
        // Address
        stream << "(0x" << std::hex << &object << std::dec;

        // Ref count
        stream << "{" << object.count() << "}: ";
    
        // Object
        if (object) {
            stream << *object;
        }
        else {
            stream << "/";
        }
        stream << ")";
        return stream;
    }
}
