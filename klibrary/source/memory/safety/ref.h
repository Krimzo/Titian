#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T, typename C = uint32_t>
    struct Ref
    {
        friend struct Ref;

        Ref()
        {}

        Ref(T* instance)
            : m_instance(instance)
        {
            if (m_instance) {
                allocate();
            }
        }

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

        Ref(const Ref& other)
            : m_instance(other.m_instance), m_count(other.m_count)
        {
            increase_count();
        }

        Ref& operator=(const Ref& other)
        {
            if (this == _addr(other))
				return *this;

            free();
            m_instance = other.m_instance;
            m_count = other.m_count;
            increase_count();
            return *this;
        }

        Ref(Ref&& other) noexcept
            : m_instance(other.m_instance), m_count(other.m_count)
        {
            other.clear();
        }

        Ref& operator=(Ref&& other) noexcept
        {
			if (this == _addr(other))
				return *this;

            free();
            m_instance = other.m_instance;
            m_count = other.m_count;
            other.clear();
            return *this;
        }

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
            if (!derived)
                return {};

            Ref<D, C> result;
            result.m_instance = derived;
            result.m_count = m_count;
            increase_count();
            return result;
        }

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

        operator bool() const
        {
            return bool(m_instance);
        }

        template<typename N = uint32_t>
        N count() const
        {
            if (m_count) {
                return N(*m_count);
            }
            return N(0);
        }

        template<typename D>
        bool is() const
        {
            return (bool) dynamic_cast<D*>(m_instance);
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

        template<typename N = uint32_t>
        N decrease_count() const
        {
            if (m_count) {
                return N(--(*m_count));
            }
            return N(-1);
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
        stream << "(0x" << std::hex << &ref << std::dec;
        stream << "{" << ref.count() << "}: ";
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
