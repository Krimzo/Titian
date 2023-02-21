#include "window/draw/frame_buffer.h"


kl::frame_buffer::frame_buffer(const int2& size)
    : buffer1_(size), buffer2_(size)
{}

kl::frame_buffer::~frame_buffer()
{}

kl::int2 kl::frame_buffer::size() const
{
    return buffer1_.size();
}

kl::image* kl::frame_buffer::back_buffer()
{
    return !first_is_front ? &buffer1_ : &buffer2_;
}

const kl::image& kl::frame_buffer::front_buffer() const
{
    return first_is_front ? buffer1_ : buffer2_;
}

void kl::frame_buffer::swap()
{
    first_is_front = !first_is_front;
}
