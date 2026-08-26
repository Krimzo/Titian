#pragma once

#include "titian.h"

namespace titian
{
using RAWImage = kl::Image;
using RAWImageType = kl::ImageType;
} // namespace titian

namespace titian
{
struct Image : kl::NoCopy
{
    RAWImage out_frame;

    Image();
    Image(StringRef const& path);

    void cache_frame(int2 size);

    Ref<Image> make_copy() const;

  private:
    RAWImage m_image;
};
} // namespace titian
