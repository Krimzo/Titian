#include "render/components/texture.h"


kl::texture::texture(kl::gpu* gpu)
    : gpu(gpu)
{}

kl::texture::texture(kl::gpu* gpu, const texture_data& data, const bool is_cube, const bool has_unordered_access, const bool is_target)
    : gpu(gpu), data_buffer(data)
{
    if (is_cube) {
        reload_as_cube();
    }
    else {
        reload(has_unordered_access, is_target);
    }
}

kl::texture::~texture()
{}

void kl::texture::reload(const bool has_unordered_access, const bool is_target)
{
    graphics_buffer = gpu->create_texture(data_buffer, has_unordered_access, is_target);
}

bool kl::texture::reload_as_cube()
{
    if (data_buffer.width() % 4 || data_buffer.height() % 3) {
        return false;
    }

    const int part_size = (data_buffer.width() / 4);
    kl::image box_sides[6] = {
        data_buffer.get_rectangle(int2(1, 1) * part_size, int2(2, 2) * part_size),
        data_buffer.get_rectangle(int2(3, 1) * part_size, int2(4, 2) * part_size),
        data_buffer.get_rectangle(int2(0, 1) * part_size, int2(1, 2) * part_size),
        data_buffer.get_rectangle(int2(2, 1) * part_size, int2(3, 2) * part_size),
        data_buffer.get_rectangle(int2(1, 0) * part_size, int2(2, 1) * part_size),
        data_buffer.get_rectangle(int2(1, 2) * part_size, int2(2, 3) * part_size),
    };

    graphics_buffer = gpu->create_cube_texture(box_sides[0], box_sides[1], box_sides[2], box_sides[3], box_sides[4], box_sides[5]);
    return true;
}

void kl::texture::create_target_view(dx::target_view_descriptor* descriptor)
{
    target_view = gpu->create_target_view(graphics_buffer, descriptor);
}

void kl::texture::create_depth_view(dx::depth_view_descriptor* descriptor)
{
    depth_view = gpu->create_depth_view(graphics_buffer, descriptor);
}

void kl::texture::create_shader_view(dx::shader_view_descriptor* descriptor)
{
    shader_view = gpu->create_shader_view(graphics_buffer, descriptor);
}

void kl::texture::create_access_view(dx::access_view_descriptor* descriptor)
{
    access_view = gpu->create_access_view(graphics_buffer, descriptor);
}
