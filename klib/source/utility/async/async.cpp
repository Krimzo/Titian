#include "klib.h"


void kl::async_for(const int64_t start_inclusive, const int64_t end_exclusive, const std::function<void(int64_t)>& loop_body)
{
    std::for_each(std::execution::par, AsyncIterator(start_inclusive), AsyncIterator(end_exclusive), loop_body);
}
