#include "klibrary.h"


kl::FrameBuffer::FrameBuffer()
{}

kl::FrameBuffer::FrameBuffer(const Int2& size)
{
    this->resize(size);
}

void kl::FrameBuffer::resize(const Int2& size)
{
    for (auto& buffer : m_buffers) {
        buffer.resize(size);
    }
}

kl::Int2 kl::FrameBuffer::size() const
{
    return m_buffers[0].size();
}

kl::Image* kl::FrameBuffer::back_buffer()
{
    return !m_first_is_front ? m_buffers : m_buffers + 1;
}

const kl::Image* kl::FrameBuffer::front_buffer() const
{
    return m_first_is_front ? m_buffers : m_buffers + 1;
}

void kl::FrameBuffer::swap()
{
    m_first_is_front = !m_first_is_front;
}
