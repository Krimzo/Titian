#include "utility/async/async.h"


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
    async_iterator() {}
    async_iterator(value_type value)
        : value_(value)
    {}

    reference operator*()
    {
        return value_;
    }

    void operator++()
    {
        value_ += 1;
    }

    void operator--()
    {
        value_ -= 1;
    }

    difference_type operator+(const async_iterator& iter) const
    {
        return (value_ + iter.value_);
    }

    difference_type operator-(const async_iterator& iter) const
    {
        return (value_ - iter.value_);
    }

    bool operator==(const async_iterator& second) const
    {
        return (value_ == second.value_);
    }
};

void kl::async::loop(const int64_t start_inclusive, const int64_t end_exclusive, const std::function<void(int64_t)>& loop_body)
{
    std::for_each(std::execution::par, async_iterator(start_inclusive), async_iterator(end_exclusive), loop_body);
}
