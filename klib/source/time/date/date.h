#pragma once

#include "apis/apis.h"


namespace kl {
    struct date
    {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        date();
    };

    std::ostream& operator<<(std::ostream& stream, const date& obj);
}
