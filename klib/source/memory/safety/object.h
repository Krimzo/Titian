#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T>
    class object
    {
        template<typename>
        friend class object;

        T* m_instance = nullptr;
        std::atomic<uint64_t>* m_count = nullptr;

        uint64_t increase_count()
        {
            return m_count ? (*m_count += 1) : 0;
        }

        uint64_t decrease_count()
        {
            return m_count ? (*m_count -= 1) : 0;
        }

        void allocate()
        {
            m_count = new std::atomic<uint64_t>;
            if (!m_count) throw std::runtime_error("Could not allocate memory for reference counter.");
            *m_count = 1;
        }

        void deallocate()
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
        object()
        {}

        object(T* instance)
            : m_instance(instance)
        {
            if (m_instance) {
                allocate();
            }
        }

        // Destroy
        virtual ~object()
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
        object(const object& other)
            : m_instance(other.m_instance), m_count(other.m_count)
        {
            increase_count();
        }

        object(const object&& other) noexcept
            : object(other)
        {}

        // Copy
        object& operator=(const object& other)
        {
            if (other.m_instance != m_instance) {
                this->free();
                m_instance = other.m_instance;
                m_count = other.m_count;
                increase_count();
            }
            return *this;
        }

        object& operator=(const object&& other) noexcept
        {
            return (*this = other);
        }

        // Derived cast
        template<typename B> requires std::is_base_of_v<B, T>
        operator object<B> ()
        {
            object<B> result = {};
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
        bool operator==(const object<O>& other) const
        {
            const void* first = m_instance;
            const void* second = other.m_instance;
            return (first == second);
        }

        template<typename O>
        bool operator!=(const object<O>& other) const
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
    std::ostream& operator<<(std::ostream& stream, const object<T>& object)
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
