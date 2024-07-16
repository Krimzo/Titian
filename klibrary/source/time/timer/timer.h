#pragma once

#include "apis/apis.h"


namespace kl {
    class Timer
    {
        uint64_t m_delta_start;
        uint64_t m_delta_end;
        uint64_t m_elapsed_start;

        float m_old_elapsed = 0.0f;
        bool m_is_running = true;

    public:
        Timer();

        void update_delta();
        float delta() const;

        void reset_elapsed();
        float elapsed() const;

        void stop();
        void restart();

        void pause();
        void resume();

        bool is_running() const;
    };
}
