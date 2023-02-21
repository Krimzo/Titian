#pragma once

#include "apis/apis.h"


namespace kl {
    class audio
    {
    public:
        audio();
        virtual ~audio();

        audio(const audio&) = delete;
        audio(const audio&&) = delete;

        void operator=(const audio&) = delete;
        void operator=(const audio&&) = delete;
    };
}
