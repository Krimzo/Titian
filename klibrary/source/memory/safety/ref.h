#pragma once

#include "apis/apis.h"


namespace kl {
    /* NOT THREAD SAFE */
    template<typename T, typename C = uint32_t>
    struct Ref
    {
        friend struct Ref;

        // create
        Ref()
        {}

        Ref(T* instance)
            : m_instance(instance)
        {
            if (m_instance) {
                allocate();
            }
        }

        // destroy
        ~Ref() noexcept
        {
            free();
        }

        void free()
        {
            if (decrease_count() == 0) {
                destroy();
            }
            clear();
        }

        // copy
        Ref(const Ref& other)
            : m_instance(other.m_instance), m_count(other.m_count)
        {
            increase_count();
        }

        Ref& operator=(const Ref& other)
        {
            if (other.m_instance != m_instance) {
                free();
                m_instance = other.m_instance;
                m_count = other.m_count;
                increase_count();
            }
            return *this;
        }

        // move
        Ref(Ref&& other) noexcept
        {
            *this = other;
            other.free();
        }

        Ref& operator=(Ref&& other) noexcept
        {
            *this = other;
            other.free();
            return *this;
        }

        // cast
        template<typename B>
            requires (not std::is_same_v<B, T> and std::is_base_of_v<B, T>)
        operator Ref<B, C>()
        {
            Ref<B, C> result;
            result.m_instance = m_instance;
            result.m_count = m_count;
            increase_count();
            return result;
        }

        template<typename D>
            requires (not std::is_same_v<D, T>)
        Ref<D, C> as() const
        {
            D* derived = dynamic_cast<D*>(m_instance);
            if (!derived) {
                return {};
            }

            Ref<D, C> result;
            result.m_instance = derived;
            result.m_count = m_count;
            increase_count();
            return result;
        }

        // compare
        bool operator==(const Ref& other) const
        {
            const void* first = m_instance;
            const void* second = other.m_instance;
            return first == second;
        }

        bool operator!=(const Ref& other) const
        {
            return !(*this == other);
        }

        // access
        T* operator&() const
        {
            return m_instance;
        }

        T& operator*() const
        {
            return *m_instance;
        }

        T* operator->() const
        {
            return m_instance;
        }

        // info
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

    private:
        T* m_instance = nullptr;
        C* m_count = nullptr;

        void increase_count() const
        {
            if (m_count) {
                ++(*m_count);
            }
        }

        template<typename T = uint32_t>
        T decrease_count() const
        {
            if (m_count) {
                return static_cast<T>(--(*m_count));
            }
            return static_cast<T>(-1);
        }

        void allocate()
        {
            m_count = new C();
            if (!m_count) {
                throw std::exception("Could not allocate memory for reference counter.");
            }
            *m_count = 1;
        }

        void destroy()
        {
            if (m_instance) {
                delete m_instance;
            }
            if (m_count) {
                delete m_count;
            }
        }

        void clear()
        {
            m_instance = nullptr;
            m_count = nullptr;
        }
    };
}

namespace kl {
    template<typename T>
    using AtomicRef = Ref<T, std::atomic<uint32_t>>;
}

namespace kl {
    template<typename T, typename C>
    std::ostream& operator<<(std::ostream& stream, const Ref<T, C>& ref)
    {
        // Address
        stream << "(0x" << std::hex << &ref << std::dec;

        // Ref count
        stream << "{" << ref.count() << "}: ";
    
        // Object
        if (ref) {
            stream << *ref;
        }
        else {
            stream << "/";
        }
        stream << ")";
        return stream;
    }
}
