#include "klibrary.h"


kl::Mesh::Mesh(kl::GPU& gpu)
    : m_gpu(gpu)
{}

void kl::Mesh::reload()
{
    graphics_buffer = m_gpu.create_vertex_buffer(data_buffer);
}
