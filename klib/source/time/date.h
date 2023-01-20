#pragma once

#include "time/time.h"


namespace kl {
    struct date
    {
        int year, month, day, hour, minute, second;

        date()
        {
            const time_t time = std::time(nullptr);
            std::tm now = {};
            localtime_s(&now, &time);

            year = now.tm_year + 1900;
            month = now.tm_mon + 1;
            day = now.tm_mday;
            hour = now.tm_hour;
            minute = now.tm_min;
            second = now.tm_sec;
        }
    };

    std::ostream& operator<<(std::ostream& stream, const date& obj);
}
