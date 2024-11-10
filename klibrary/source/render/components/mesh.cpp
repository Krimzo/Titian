#include "klibrary.h"


kl::Mesh::Mesh( kl::GPU const& gpu )
    : m_gpu( gpu )
{}

void kl::Mesh::reload()
{
    buffer = m_gpu.create_vertex_buffer( vertices );
}
