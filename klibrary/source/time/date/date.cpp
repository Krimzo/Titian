#include "klibrary.h"


kl::Date::Date()
{
    std::tm date_now{};
    const time_t current_time = std::time(nullptr);
    localtime_s(&date_now, &current_time);
    year = date_now.tm_year + 1900;
    month = date_now.tm_mon + 1;
    day = date_now.tm_mday;
    hour = date_now.tm_hour;
    minute = date_now.tm_min;
    second = date_now.tm_sec;

    namespace ch = std::chrono;
    const auto time_since_epoch = ch::system_clock::now().time_since_epoch();
    milisecond = int(ch::duration_cast<ch::milliseconds>(time_since_epoch).count() % 1000);
}

std::ostream& kl::operator<<(std::ostream& stream, const Date& obj)
{
    stream << std::setfill('0') <<
        std::setw(4) << obj.year << '.' <<
        std::setw(2) << obj.month << '.' <<
        std::setw(2) << obj.day << ' ' <<
        std::setw(2) << obj.hour << ':' <<
        std::setw(2) << obj.minute << ':' <<
        std::setw(2) << obj.second << '.' <<
        std::setw(3) << obj.milisecond;
    return stream;
}
