#include "klibrary.h"


kl::RenderShaders::operator bool() const
{
    return (vertex_shader && pixel_shader);
}
