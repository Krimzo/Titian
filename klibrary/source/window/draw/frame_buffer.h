#pragma once

#include "media/media.h"


namespace kl {
    class FrameBuffer
    {
        bool m_first_is_front = true;
        Image m_buffers[2] = {};

    public:
        FrameBuffer();
        FrameBuffer(const Int2& size);

        void resize(const Int2& size);
        Int2 size() const;

        Image* back_buffer();
        const Image* front_buffer() const;

        void swap();
    };
}
