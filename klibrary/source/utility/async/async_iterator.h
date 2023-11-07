#pragma once

#include "apis/apis.h"


namespace kl {
    class AsyncIterator
    {
    public:
        using value_type = int64_t;
        using pointer = value_type*;
        using reference = value_type&;

        using iterator_category = std::random_access_iterator_tag;
        using difference_type = value_type;

    private:
        value_type m_value = {};

    public:
        AsyncIterator();
        AsyncIterator(value_type value);

        void operator++();
        void operator--();

        difference_type operator+(const AsyncIterator& iter) const;
        difference_type operator-(const AsyncIterator& iter) const;

        reference operator*();
        bool operator==(const AsyncIterator& second) const;
    };
}
