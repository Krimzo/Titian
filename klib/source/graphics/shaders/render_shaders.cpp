#include "graphics/shaders/render_shaders.h"


// Creation
kl::render_shaders::render_shaders()
{}

kl::render_shaders::~render_shaders()
{}

// Get
kl::render_shaders::operator bool() const
{
    return (input_layout && vertex_shader && pixel_shader);
}
