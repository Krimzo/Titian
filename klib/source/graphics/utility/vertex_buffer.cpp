#include "graphics/gpu.h"

#include "memory/file.h"


kl::dx::buffer kl::gpu::new_vertex_buffer(const std::vector<vertex>& vertex_data)
{
    dx::buffer_descriptor descriptor = {};
    descriptor.ByteWidth = UINT(vertex_data.size() * sizeof(vertex));
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    dx::subresource_descriptor data = {};
    data.pSysMem = vertex_data.data();

    return new_buffer(&descriptor, &data);
}

kl::dx::buffer kl::gpu::new_vertex_buffer(const std::string& filepath, const bool flip_z)
{
    return new_vertex_buffer(files::parse_mesh(filepath, flip_z));
}

kl::dx::buffer kl::gpu::generate_screen_mesh()
{
    return new_vertex_buffer({
        vertex({ -1, -1, 0.5f }, float2(0, 0)), vertex({ -1,  1, 0.5f }, float2(0, 1)), vertex({  1,  1, 0.5f }, float2(1, 1)),
        vertex({  1,  1, 0.5f }, float2(1, 1)), vertex({  1, -1, 0.5f }, float2(1, 0)), vertex({ -1, -1, 0.5f }, float2(0, 0))
        });
}

kl::dx::buffer kl::gpu::generate_plane_mesh(const float size, int num_of_points)
{
    // Count fix
    const float increment = size / num_of_points;
    num_of_points += 1;

    // Generating points
    std::vector<float2> points = {};
    points.reserve(num_of_points * num_of_points);
    for (int x = 0; x < num_of_points; x++) {
        for (int z = 0; z < num_of_points; z++) {
            points.emplace_back(
                -size * 0.5f + increment * x,
                -size * 0.5f + increment * z
            );
        }
    }

    // Generating triangles
    std::vector<vertex> vertices = {};
    vertices.reserve((num_of_points - 1) * (num_of_points - 1) * 6);
    for (int x = 0; x < num_of_points - 1; x++) {
        for (int z = 0; z < num_of_points - 1; z++) {
            const float2& xz_point = points[z * num_of_points + x];
            const float2& xz_plus_point = points[(z + 1) * num_of_points + (x + 1)];

            vertices.push_back(vertex({ xz_plus_point.x, 0, xz_plus_point.y }));
            vertices.push_back(vertex({ xz_plus_point.x, 0, xz_point.y }));
            vertices.push_back(vertex({ xz_point.x, 0, xz_point.y }));

            vertices.push_back(vertex({ xz_point.x, 0, xz_point.y }));
            vertices.push_back(vertex({ xz_point.x, 0, xz_plus_point.y }));
            vertices.push_back(vertex({ xz_plus_point.x, 0, xz_plus_point.y }));
        }
    }
    return new_vertex_buffer(vertices);
}

void kl::gpu::draw_vertex_buffer(const dx::buffer buffer) const
{
    static constexpr UINT offset = 0;
    static constexpr UINT stride = sizeof(vertex);

    dx::buffer_descriptor descriptor = {};
    buffer->GetDesc(&descriptor);

    context_->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    context_->Draw(descriptor.ByteWidth / stride, 0);
}
