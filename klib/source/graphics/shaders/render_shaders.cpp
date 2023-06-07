#include "graphics/shaders/render_shaders.h"


kl::render_shaders::operator bool() const
{
    return (vertex_shader && pixel_shader);
}
