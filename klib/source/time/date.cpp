#include "time/date.h"


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
