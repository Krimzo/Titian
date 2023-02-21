#pragma once

#include "apis/apis.h"


namespace kl {
    class timer
    {
        time_t interval_start_;
        time_t interval_end_;
        time_t elapsed_start_;

    public:
        timer();

        void reset();

        void update_interval();
        float get_interval() const;

        void update_elapsed();
        float get_elapsed() const;
    };
}
