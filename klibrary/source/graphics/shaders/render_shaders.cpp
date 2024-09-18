#include "klibrary.h"


kl::RenderShaders::RenderShaders(const GPU* gpu)
    : CBuffer(gpu)
{}

kl::RenderShaders::operator bool() const
{
    return gpu && vertex_shader && pixel_shader;
}
