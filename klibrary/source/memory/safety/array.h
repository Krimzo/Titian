#pragma once

#include "apis/apis.h"


namespace kl {
    /* NOT THREAD SAFE */
    template<typename T, typename C = uint32_t>
    class Array
    {
        T* m_data = nullptr;
        C* m_count = nullptr;
        size_t m_size = 0;

        inline void increase_count() const
        {
            if (m_count) {
                ++(*m_count);
            }
        }

        inline uint32_t decrease_count() const
        {
            if (m_count) {
                return --(*m_count);
            }
            return -1;
        }

        inline void allocate()
        {
            // Data
            m_data = new T[m_size]();
            if (!m_data) {
                throw std::exception("Could not allocate memory for array data.");
            }

            // Counter
            m_count = new C();
            if (!m_count) {
                throw std::exception("Could not allocate memory for reference counter.");
            }
            *m_count = 1;
        }

        inline void deallocate()
        {
            if (m_data) {
                delete[] m_data;
                m_data = nullptr;
            }
            if (m_count) {
                delete m_count;
                m_count = nullptr;
            }
            m_size = 0;
        }

        inline void clear()
        {
            m_data = nullptr;
            m_count = nullptr;
            m_size = 0;
        }

    public:
        // Create
        Array()
        {}

        Array(size_t size)
            : m_size(size)
        {
            allocate();
        }

        // Destroy
        virtual ~Array()
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
        Array(const Array& other)
            : m_data(other.m_data), m_count(other.m_count), m_size(other.m_size)
        {
            increase_count();
        }

        Array(Array&& other) noexcept
            : Array(other)
        {}

        // Copy
        Array& operator=(const Array& other)
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

        Array& operator=(Array&& other) noexcept
        {
            return (*this = other);
        }

        // Info
        operator bool() const
        {
            return static_cast<bool>(m_data);
        }

        template<typename T = uint32_t>
        T count() const
        {
            if (m_count) {
                return static_cast<T>(*m_count);
            }
            return static_cast<T>(0);
        }

        size_t size() const
        {
            return m_size;
        }

        size_t byte_size() const
        {
            return m_size * sizeof(T);
        }

        bool empty() const
        {
            return m_size == 0;
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
        bool operator==(const Array& other) const
        {
            return (m_data == other.m_data);
        }

        bool operator!=(const Array& other) const
        {
            return !(*this == other);
        }

        // Access
        T* operator&()
        {
            return m_data;
        }

        const T* operator&() const
        {
            return m_data;
        }

        T& operator[](size_t index)
        {
            return m_data[index];
        }

        const T& operator[](size_t index) const
        {
            return m_data[index];
        }
    };
}

namespace kl {
    template<typename T>
    using SafeArray = Array<T, std::atomic<uint32_t>>;
}

namespace kl {
    template<typename T, typename C>
    std::ostream& operator<<(std::ostream& stream, const Array<T, C>& array)
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
            for (size_t i = 0; i < array.size() - 1; i++) {
                stream << array[i] << ", ";
            }
            stream << array[array.size() - 1] << "]";
        }
        stream << ")";
        return stream;
    }
}
