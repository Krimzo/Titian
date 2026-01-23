#include "klibrary.h"


kl::Date::Date( bool local_time )
{
    std::tm date_now{};
    time_t current_time = std::time( nullptr );
    ( local_time ? localtime_s : gmtime_s )( &date_now, &current_time );
    year = date_now.tm_year + 1900;
    month = date_now.tm_mon + 1;
    day = date_now.tm_mday;
    hour = date_now.tm_hour;
    minute = date_now.tm_min;
    second = date_now.tm_sec;
    milisecond = int( ch::duration_cast<ch::milliseconds>( ch::system_clock::now().time_since_epoch() ).count() % 1000 );
}

float kl::Date::year_perc() const
{
    static constexpr uint64_t month_seconds[12] = {
        31 * 24 * 3600, // January
        28 * 24 * 3600, // February
        31 * 24 * 3600, // March
        30 * 24 * 3600, // April
        31 * 24 * 3600, // May
        30 * 24 * 3600, // June
        31 * 24 * 3600, // July
        31 * 24 * 3600, // August
        30 * 24 * 3600, // September
        31 * 24 * 3600, // October
        30 * 24 * 3600, // November
        31 * 24 * 3600, // December
    };
    static constexpr uint64_t year_seconds = 365 * 24 * 3600;

    uint64_t passed_seconds = 0;
    for ( int i = 0; i < ( month - 1 ); i++ )
        passed_seconds += month_seconds[i];
    passed_seconds += uint64_t( day - 1 ) * 24 * 3600;
    passed_seconds += uint64_t( hour ) * 3600;
    passed_seconds += uint64_t( minute ) * 60;
    passed_seconds += second;

    return float( double( passed_seconds ) / year_seconds );
}

float kl::Date::day_perc() const
{
    static constexpr uint64_t day_ms = 24 * 3600 * 1000;

    uint64_t passed_ms = 0;
    passed_ms += uint64_t( hour ) * 3600 * 1000;
    passed_ms += uint64_t( minute ) * 60 * 1000;
    passed_ms += uint64_t( second ) * 1000;
    passed_ms += milisecond;

    return float( double( passed_ms ) / day_ms );
}

std::ostream& kl::operator<<( std::ostream& stream, Date const& obj )
{
    stream << std::setfill( '0' ) <<
        std::setw( 4 ) << obj.year << '.' <<
        std::setw( 2 ) << obj.month << '.' <<
        std::setw( 2 ) << obj.day << ' ' <<
        std::setw( 2 ) << obj.hour << ':' <<
        std::setw( 2 ) << obj.minute << ':' <<
        std::setw( 2 ) << obj.second << '.' <<
        std::setw( 3 ) << obj.milisecond;
    return stream;
}
