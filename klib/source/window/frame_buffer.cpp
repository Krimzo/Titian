#include "window/frame_buffer.h"


kl::frame_buffer::frame_buffer(const int2& size)
    : buffer1_(size), buffer2_(size)
{
    front_ = &buffer1_;
}

kl::frame_buffer::~frame_buffer()
{}

kl::int2 kl::frame_buffer::size() const
{
    return buffer1_.size();
}

kl::image* kl::frame_buffer::back_buffer()
{
    if (front_ == &buffer1_) {
        return &buffer2_;
    }
    return &buffer1_;
}

void kl::frame_buffer::display(const window& window)
{
    lock_.lock();
    window.draw_image(*front_);
    lock_.unlock();
}

void kl::frame_buffer::swap()
{
    lock_.lock();
    front_ = back_buffer();
    lock_.unlock();
}
