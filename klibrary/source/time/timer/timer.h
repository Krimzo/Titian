#pragma once

#include "apis/apis.h"


namespace kl {
    struct Timer
    {
        Timer();

        void update();

        void stop();
        void start();

        void reset();
        void restart();

        bool active() const;
        float delta() const;
        float elapsed() const;

    private:
        uint64_t m_last = 0;
        float m_delta = 0.0f;
        float m_elapsed = 0.0f;
        bool m_active = false;
    };
}
