#include "time/date/date.h"


kl::date::date()
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

std::ostream& kl::operator<<(std::ostream& stream, const date& obj)
{
    stream << obj.year <<
        '.' << obj.month <<
        '.' << obj.day <<
        ' ' << obj.hour <<
        ':' << obj.minute <<
        ':' << obj.second;
    return stream;
}
