#include "klib.h"


kl::RenderShaders::operator bool() const
{
    return (vertex_shader && pixel_shader);
}
