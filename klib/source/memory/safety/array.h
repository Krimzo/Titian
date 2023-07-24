#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T>
    class array
    {
        T* m_data = nullptr;
        std::atomic<uint64_t>* m_count = nullptr;
        uint64_t m_size = 0;

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
            // Data
            m_data = new T[m_size]();
            if (!m_data) throw std::runtime_error("Could not allocate memory for array data.");

            // Counter
            m_count = new std::atomic<uint64_t>;
            if (!m_count) throw std::runtime_error("Could not allocate memory for reference counter.");
            *m_count = 1;
        }

        void deallocate()
        {
            if (m_data) delete[] m_data;
            if (m_count) delete m_count;
        }

        void clear()
        {
            m_data = nullptr;
            m_count = nullptr;
            m_size = 0;
        }

    public:
        // Create
        array()
        {}

        array(const uint64_t size)
            : m_size(size)
        {
            allocate();
        }

        // Destroy
        ~array()
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
        array(const array<T>& other)
            : m_data(other.m_data), m_count(other.m_count), m_size(other.m_size)
        {
            increase_count();
        }

        array(const array<T>&& other) noexcept
            : array<T>(other)
        {}

        // Copy
        array<T>& operator=(const array<T>& other)
        {
            if (other.m_data != m_data) {
                this->free();
                m_data = other.m_data;
                m_count = other.m_count;
                m_size = other.m_size;
                increase_count();
            }
            return *this;
        }

        array<T>& operator=(const array<T>&& other) noexcept
        {
            return (*this = other);
        }

        // Info
        operator bool() const
        {
            return (bool) m_data;
        }

        uint64_t count() const
        {
            return m_count ? m_count->load() : 0;
        }

        uint64_t size() const
        {
            return m_size;
        }

        uint64_t byte_size() const
        {
            return m_size * sizeof(T);
        }

        bool empty() const
        {
            return (m_size == 0);
        }

        // Iterate
        T* begin()
        {
            return m_data;
        }

        const T* begin() const
        {
            return m_data;
        }

        T* end()
        {
            return (m_data + m_size);
        }

        const T* end() const
        {
            return (m_data + m_size);
        }

        // Compare
        bool operator==(const array<T>& other) const
        {
            return (m_data == other.m_data);
        }

        bool operator!=(const array<T>& other) const
        {
            return !(*this == other);
        }

        // Address
        T* operator&()
        {
            return m_data;
        }

        const T* operator&() const
        {
            return m_data;
        }

        // Access
        T& operator*()
        {
            return *m_data;
        }

        const T& operator*() const
        {
            return *m_data;
        }

        T* operator->()
        {
            return m_data;
        }

        const T* operator->() const
        {
            return m_data;
        }

        T& operator[](const uint64_t index)
        {
            return m_data[index];
        }

        const T& operator[](const uint64_t index) const
        {
            return m_data[index];
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const array<T>& array)
    {
        // Address
        stream << "(0x" << std::hex << &array << std::dec;

        // Ref count
        stream << "{" << array.count() << "}: ";

        // Data
        if (array.empty()) {
            stream << "[]";
        }
        else {
            stream << "[";
            for (uint64_t i = 0; i < array.size() - 1; i++) {
                stream << array[i] << ", ";
            }
            stream << array[array.size() - 1] << "]";
        }
        stream << ")";
        return stream;
    }
}
