#pragma once

#include "apis/apis.h"


namespace kl {
    /* NOT THREAD SAFE */
    template<typename T, typename C = uint32_t>
    class Object
    {
        template<typename, typename>
        friend class Object;

        T* m_instance = nullptr;
        C* m_count = nullptr;

        inline void increase_count() const
        {
            if (m_count) {
                ++(*m_count);
            }
        }

        template<typename T = uint32_t>
        inline T decrease_count() const
        {
            if (m_count) {
                return static_cast<T>(--(*m_count));
            }
            return static_cast<T>(-1);
        }

        inline void allocate()
        {
            m_count = new C();
            if (!m_count) {
                throw std::exception("Could not allocate memory for reference counter.");
            }
            *m_count = 1;
        }

        inline void deallocate()
        {
            if (m_instance) {
                delete m_instance;
                m_instance = nullptr;
            }
            if (m_count) {
                delete m_count;
                m_count = nullptr;
            }
        }

        inline void clear()
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
        ~Object()
        {
            this->free();
        }

        void free()
        {
            if (decrease_count() == 0) {
                deallocate();
            }
            else {
                clear();
            }
        }

        // Create copy
        Object(const Object& other)
            : m_instance(other.m_instance), m_count(other.m_count)
        {
            increase_count();
        }

        Object(Object&& other) noexcept
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

        // Cast
        template<typename B>
            requires (not std::is_same_v<B, T> and std::is_base_of_v<B, T>)
        operator Object<B, C> ()
        {
            Object<B, C> result;
            result.m_instance = m_instance;
            result.m_count = m_count;
            increase_count();
            return result;
        }

        template<typename D>
            requires (not std::is_same_v<D, T>)
        Object<D, C> as() const
        {
            D* derived = dynamic_cast<D*>(m_instance);
            if (!derived) {
                return {};
            }

            Object<D, C> result;
            result.m_instance = derived;
            result.m_count = m_count;
            increase_count();
            return result;
        }

        // Info
        operator bool() const
        {
            return static_cast<bool>(m_instance);
        }

        template<typename T = uint32_t>
        T count() const
        {
            if (m_count) {
                return static_cast<T>(*m_count);
            }
            return static_cast<T>(0);
        }

        template<typename T>
        bool is() const
        {
            return static_cast<bool>(
                dynamic_cast<T*>(m_instance));
        }

        // Compare
        bool operator==(const Object& other) const
        {
            const void* first = m_instance;
            const void* second = other.m_instance;
            return first == second;
        }

        bool operator!=(const Object& other) const
        {
            return !(*this == other);
        }

        // Access
        T* operator&()
        {
            return m_instance;
        }

        const T* operator&() const
        {
            return m_instance;
        }

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
    using SafeObject = Object<T, std::atomic<uint32_t>>;
}

namespace kl {
    template<typename T, typename C>
    std::ostream& operator<<(std::ostream& stream, const Object<T, C>& object)
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
