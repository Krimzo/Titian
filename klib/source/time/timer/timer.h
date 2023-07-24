#pragma once

#include "apis/apis.h"


namespace kl {
    class timer
    {
        time_t delta_start_;
        time_t delta_end_;
        time_t elapsed_start_;

    public:
        timer();

        void reset();

        void update_delta();
        float delta() const;

        void reset_elapsed();
        float elapsed() const;
    };
}
