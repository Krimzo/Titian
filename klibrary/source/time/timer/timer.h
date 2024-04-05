#pragma once

#include "apis/apis.h"


namespace kl {
    class Timer
    {
        uint64_t m_delta_start;
        uint64_t m_delta_end;
        uint64_t m_elapsed_start;

        float m_old_elapsed = 0.0f;
        bool m_is_paused = false;

    public:
        Timer();

        void reset();

        void update_delta();
        float delta() const;

        void reset_elapsed();
        float elapsed() const;

        bool is_paused() const;
        void pause();
        void resume();
    };
}
