#pragma once

#include "apis/kl_cpp.h"


namespace kl {
    constexpr const void* _addr(const auto& obj)
    {
        return &reinterpret_cast<const byte&>(obj);
    }
}

namespace kl {
    template<typename T>
    struct ComRef
    {
        ComRef()
        {}

        explicit ComRef(T* instance)
            : m_instance(instance)
        {}

        ~ComRef() noexcept
        {
            free();
        }

        void free()
        {
            destroy();
            clear();
        }

        ComRef(const ComRef& other)
            : m_instance(other.m_instance)
        {
            increase_count();
        }

        ComRef& operator=(const ComRef& other)
        {
            if (this == _addr(other))
                return *this;

            free();
            m_instance = other.m_instance;
            increase_count();
            return *this;
        }

        ComRef(ComRef&& other) noexcept
            : m_instance(other.m_instance)
        {
            other.clear();
        }

        ComRef& operator=(ComRef&& other) noexcept
        {
            if (this == _addr(other))
                return *this;

            free();
            m_instance = other.m_instance;
            other.clear();
            return *this;
        }

        template<typename B>
            requires (not std::is_same_v<B, T> and std::is_base_of_v<B, T>)
        operator ComRef<B>() const
        {
            increase_count();
            return ComRef<B>{m_instance};
        }

        template<typename D>
            requires (not std::is_same_v<D, T>)
        HRESULT as(ComRef<D>& out) const
        {
            return m_instance->QueryInterface(__uuidof(D), (void**) &out);
        }

        T** operator&()
        {
            free();
            return &m_instance;
        }

        T& operator*() const
        {
            return *m_instance;
        }

        T* operator->() const
        {
            return m_instance;
        }

        T* get() const
        {
            return m_instance;
        }

        T* const* address() const
        {
            return &m_instance;
        }

        operator bool() const
        {
            return bool(m_instance);
        }

    private:
        T* m_instance = nullptr;

        void increase_count() const
        {
            if (m_instance) {
                m_instance->AddRef();
            }
        }

        void destroy()
        {
            if (m_instance) {
                m_instance->Release();
            }
        }

        void clear()
        {
            m_instance = nullptr;
        }
    };
}
