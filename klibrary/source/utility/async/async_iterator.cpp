#include "klibrary.h"


kl::AsyncIterator::AsyncIterator()
{}

kl::AsyncIterator::AsyncIterator(value_type value)
    : m_value(value)
{}

void kl::AsyncIterator::operator++()
{
    m_value += 1;
}

void kl::AsyncIterator::operator--()
{
    m_value -= 1;
}

kl::AsyncIterator::difference_type kl::AsyncIterator::operator+(const AsyncIterator& iter) const
{
    return (m_value + iter.m_value);
}

kl::AsyncIterator::difference_type kl::AsyncIterator::operator-(const AsyncIterator& iter) const
{
    return (m_value - iter.m_value);
}

kl::AsyncIterator::reference kl::AsyncIterator::operator*()
{
    return m_value;
}

bool kl::AsyncIterator::operator==(const AsyncIterator& second) const
{
    return (m_value == second.m_value);
}
