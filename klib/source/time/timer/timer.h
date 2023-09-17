#pragma once

#include "apis/apis.h"


namespace kl {
    class Timer
    {
        time_t m_delta_start;
        time_t m_delta_end;
        time_t m_elapsed_start;

    public:
        Timer();

        void reset();

        void update_delta();
        float delta() const;

        void reset_elapsed();
        float elapsed() const;
    };
}
