#include "time/time.h"


uint64_t kl::time::now()
{
    uint64_t result = 0;
    QueryPerformanceCounter((LARGE_INTEGER*) &result);
    return result;
}

uint64_t kl::time::cpu_frequency()
{
    uint64_t result = 0;
    QueryPerformanceFrequency((LARGE_INTEGER*) &result);
    return result;
}

float kl::time::calculate(const uint64_t start, const uint64_t end)
{
    static const float rec_frequency = (1.0f / cpu_frequency());
    return (end - start) * rec_frequency;
}

float kl::time::get_interval()
{
    static uint64_t start_time = now();

    const uint64_t end_time = now();
    const float elapsed_time = calculate(start_time, end_time);

    start_time = end_time;
    return elapsed_time;
}

void kl::time::wait(const float seconds)
{
    const uint64_t start_time = now();
    while (calculate(start_time, now()) < seconds);
}

bool kl::time::sleep(const float seconds)
{
    const HANDLE timer = CreateWaitableTimerA(nullptr, true, nullptr);
    if (!timer) {
        return false;
    }

    static const time_t frequency = cpu_frequency();
    const time_t to_sleep = -time_t(seconds * frequency);
    if (!SetWaitableTimer(timer, (const LARGE_INTEGER*) &to_sleep, 0, nullptr, nullptr, false)) {
        CloseHandle(timer);
        return false;
    }

    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
    return true;
}
