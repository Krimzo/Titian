#include "klibrary.h"


uint64_t kl::time::cpu_frequency()
{
    uint64_t result{};
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&result));
    return result;
}

uint64_t kl::time::now()
{
    uint64_t result{};
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&result));
    return result;
}

float kl::time::elapsed(const uint64_t start, const uint64_t end)
{
    static const float rec_frequency = (1.0f / cpu_frequency());
    return (end - start) * rec_frequency;
}

float kl::time::elapsed(const uint64_t from)
{
    return elapsed(from, now());
}

float kl::time::delta()
{
    static uint64_t start_time = now();
    const uint64_t end_time = now();
    const float elapsed_time = elapsed(start_time, end_time);
    start_time = end_time;
    return elapsed_time;
}

void kl::time::wait(const float seconds)
{
    const uint64_t start_time = now();
    while (elapsed(start_time) < seconds);
}

void kl::time::sleep(const float seconds)
{
    Sleep(DWORD(seconds * 1000));
}
