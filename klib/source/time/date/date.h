#pragma once

#include "apis/apis.h"


namespace kl {
    class Date
    {
    public:
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        Date();
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Date& obj);
}
