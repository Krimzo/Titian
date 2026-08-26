#pragma once

#include "storage/video.h"

namespace titian
{
struct Frame : kl::NoCopy
{
    dx::Texture texture;
    dx::ShaderView shader_view;
    dx::AccessView access_view;

    Frame();

    void upload(RAWImage const& image);
    void retrieve(RAWImage& image) const;

    int2 size() const;
    void resize(int2 size);

  private:
    dx::Texture m_staging_texture;
    int2 m_size;
};
} // namespace titian
