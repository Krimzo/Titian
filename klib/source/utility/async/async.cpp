#include "utility/async/async.h"


class custom_iterator
{
public:
    using value_type = int64_t;
    using pointer = value_type*;
    using reference = value_type&;

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

private:
    value_type value = 0;

public:
    custom_iterator() {}

    custom_iterator(value_type value) : value(value) {}

    reference operator*()
    {
        return value;
    }

    pointer operator->()
    {
        return &value;
    }

    custom_iterator& operator++()
    {
        value += 1;
        return *this;
    }

    custom_iterator operator++(int)
    {
        custom_iterator temp = *this;
        ++(*this);
        return temp;
    }

    friend bool operator==(const custom_iterator& first, const custom_iterator& second)
    {
        return first.value == second.value;
    }

    friend bool operator!=(const custom_iterator& first, const custom_iterator& second)
    {
        return first.value != second.value;
    }
};

void kl::async::loop(const int64_t start_inclusive, const int64_t end_exclusive, const std::function<void(int64_t)>& loop_body)
{
    std::for_each(std::execution::par, custom_iterator(start_inclusive), custom_iterator(end_exclusive), loop_body);
}
