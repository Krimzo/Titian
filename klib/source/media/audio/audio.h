#pragma once


namespace kl {
    class audio
    {
    public:
        audio();
        ~audio();

        audio(const audio&) = delete;
        audio(const audio&&) = delete;

        void operator=(const audio&) = delete;
        void operator=(const audio&&) = delete;
    };
}
