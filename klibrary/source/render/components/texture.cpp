#include "klibrary.h"


kl::Texture::Texture(GPU* gpu)
    : m_gpu(gpu)
{}

void kl::Texture::reload(const bool has_unordered_access, const bool is_target)
{
    graphics_buffer = m_gpu->create_texture(data_buffer, has_unordered_access, is_target);
    is_cube = false;
}

bool kl::Texture::reload_as_cube()
{
    if (data_buffer.width() % 4 || data_buffer.height() % 3) {
        return false;
    }

    const int part_size = (data_buffer.width() / 4);
    const Image box_sides[6] = {
        data_buffer.rectangle(Int2(1, 1) * part_size, Int2(2, 2) * part_size),
        data_buffer.rectangle(Int2(3, 1) * part_size, Int2(4, 2) * part_size),
        data_buffer.rectangle(Int2(0, 1) * part_size, Int2(1, 2) * part_size),
        data_buffer.rectangle(Int2(2, 1) * part_size, Int2(3, 2) * part_size),
        data_buffer.rectangle(Int2(1, 0) * part_size, Int2(2, 1) * part_size),
        data_buffer.rectangle(Int2(1, 2) * part_size, Int2(2, 3) * part_size),
    };

    graphics_buffer = m_gpu->create_cube_texture(box_sides[0], box_sides[1], box_sides[2], box_sides[3], box_sides[4], box_sides[5]);
    is_cube = true;
    return true;
}

void kl::Texture::create_target_view(const dx::TargetViewDescriptor* descriptor)
{
    target_view = m_gpu->create_target_view(graphics_buffer, descriptor);
}

void kl::Texture::create_depth_view(const dx::DepthViewDescriptor* descriptor)
{
    depth_view = m_gpu->create_depth_view(graphics_buffer, descriptor);
}

void kl::Texture::create_shader_view(const dx::ShaderViewDescriptor* descriptor)
{
    shader_view = m_gpu->create_shader_view(graphics_buffer, descriptor);
}

void kl::Texture::create_access_view(const dx::AccessViewDescriptor* descriptor)
{
    access_view = m_gpu->create_access_view(graphics_buffer, descriptor);
}
