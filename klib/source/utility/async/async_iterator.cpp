#include "utility/async/async_iterator.h"


kl::async_iterator::async_iterator()
{}

kl::async_iterator::async_iterator(value_type value)
    : value_(value)
{}

void kl::async_iterator::operator++()
{
    value_ += 1;
}

void kl::async_iterator::operator--()
{
    value_ -= 1;
}

kl::async_iterator::difference_type kl::async_iterator::operator+(const async_iterator& iter) const
{
    return (value_ + iter.value_);
}

kl::async_iterator::difference_type kl::async_iterator::operator-(const async_iterator& iter) const
{
    return (value_ - iter.value_);
}

kl::async_iterator::reference kl::async_iterator::operator*()
{
    return value_;
}

bool kl::async_iterator::operator==(const async_iterator& second) const
{
    return (value_ == second.value_);
}
