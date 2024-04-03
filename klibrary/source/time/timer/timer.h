#pragma once

#include "apis/apis.h"


namespace kl {
    class Timer
    {
        uint64_t m_delta_start;
        uint64_t m_delta_end;
        uint64_t m_elapsed_start;

    public:
        Timer();

        void reset();

        void update_delta();
        float delta() const;

        void reset_elapsed();
        float elapsed() const;
    };
}
