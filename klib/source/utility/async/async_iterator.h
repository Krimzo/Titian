#pragma once

#include "apis/apis.h"


namespace kl {
    class async_iterator
    {
    public:
        using value_type = int64_t;
        using pointer = value_type*;
        using reference = value_type&;

        using iterator_category = std::random_access_iterator_tag;
        using difference_type = value_type;

    private:
        value_type value_ = {};

    public:
        async_iterator();
        async_iterator(value_type value);

        void operator++();
        void operator--();

        difference_type operator+(const async_iterator& iter) const;
        difference_type operator-(const async_iterator& iter) const;

        reference operator*();
        bool operator==(const async_iterator& second) const;
    };
}
