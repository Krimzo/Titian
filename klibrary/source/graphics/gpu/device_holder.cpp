#include "klibrary.h"


kl::dx::RasterState kl::DeviceHolder::create_raster_state(const dx::RasterStateDescriptor* descriptor) const
{
    dx::RasterState state;
    const long result = m_device->CreateRasterizerState(descriptor, &state);
    verify(state, "Failed to create rasterizer state. Result: 0x", std::hex, result);
    return state;
}

kl::dx::RasterState kl::DeviceHolder::create_raster_state(const bool wireframe, const bool cull, const bool cull_back) const
{
    dx::RasterStateDescriptor descriptor{};
    descriptor.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    descriptor.CullMode = cull ? (cull_back ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
    descriptor.MultisampleEnable = true;
    descriptor.AntialiasedLineEnable = true;
    descriptor.DepthClipEnable = true;
    return create_raster_state(&descriptor);
}

kl::dx::DepthState kl::DeviceHolder::create_depth_state(const dx::DepthStateDescriptor* descriptor) const
{
    dx::DepthState state;
    const long result = m_device->CreateDepthStencilState(descriptor, &state);
    verify(state, "Failed to create depth stencil state. Result: 0x", std::hex, result);
    return state;
}

kl::dx::DepthState kl::DeviceHolder::create_depth_state(const bool depth, const bool stencil, const bool mask) const
{
    dx::DepthStateDescriptor descriptor{};
    if (depth) {
        descriptor.DepthEnable = true;
        descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        descriptor.DepthFunc = D3D11_COMPARISON_LESS;
    }
    if (stencil) {
        descriptor.StencilEnable = true;
        descriptor.StencilReadMask = 0xFF;
        descriptor.StencilWriteMask = 0xFF;
        if (mask) {
            descriptor.DepthEnable = false;
            descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
            descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
        }
        else {
            descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
            descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        }
    }
    return create_depth_state(&descriptor);
}

kl::dx::SamplerState kl::DeviceHolder::create_sampler_state(const dx::SamplerStateDescriptor* descriptor) const
{
    dx::SamplerState state;
    const long result = m_device->CreateSamplerState(descriptor, &state);
    verify(state, "Failed to create sampler state. Result: 0x", std::hex, result);
    return state;
}

kl::dx::SamplerState kl::DeviceHolder::create_sampler_state(const bool linear, const bool mirror) const
{
    dx::SamplerStateDescriptor descriptor{};
    descriptor.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    descriptor.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    descriptor.AddressV = descriptor.AddressU;
    descriptor.AddressW = descriptor.AddressU;
    return create_sampler_state(&descriptor);
}

kl::dx::BlendState kl::DeviceHolder::create_blend_state(const dx::BlendStateDescriptor* descriptor) const
{
    dx::BlendState state;
    const long result = m_device->CreateBlendState(descriptor, &state);
    verify(state, "Failed to create blend state. Result: 0x", std::hex, result);
    return state;
}

kl::dx::BlendState kl::DeviceHolder::create_blend_state(const bool transparency) const
{
    dx::BlendStateDescriptor descriptor{};
    descriptor.RenderTarget[0].BlendEnable = transparency;
    descriptor.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    descriptor.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    descriptor.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    descriptor.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    descriptor.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    descriptor.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    descriptor.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    return create_blend_state(&descriptor);
}

kl::dx::Buffer kl::DeviceHolder::create_buffer(const dx::BufferDescriptor* descriptor, const dx::SubresourceDescriptor* subresource_data) const
{
    dx::Buffer buffer;
    const long result = m_device->CreateBuffer(descriptor, subresource_data, &buffer);
    verify(buffer, "Failed to create gpu buffer. Result: 0x", std::hex, result);
    return buffer;
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(const void* data, const UINT byte_size) const
{
    dx::BufferDescriptor descriptor{};
    descriptor.ByteWidth = byte_size;
    descriptor.Usage = D3D11_USAGE_IMMUTABLE;
    descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    dx::SubresourceDescriptor subresource_data{};
    subresource_data.pSysMem = data;
    return create_buffer(&descriptor, &subresource_data);
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(const std::vector<Vertex>& vertices) const
{
    return create_vertex_buffer(vertices.data(), UINT(vertices.size() * sizeof(Vertex)));
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(const std::vector<Triangle>& triangles) const
{
    return create_vertex_buffer(triangles.data(), UINT(triangles.size() * sizeof(Triangle)));
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(const std::string_view& filepath, const bool flip_z) const
{
    return create_vertex_buffer(parse_obj_file(filepath, flip_z));
}

kl::dx::Buffer kl::DeviceHolder::create_index_buffer(const uint32_t* data, const UINT element_count) const
{
    dx::BufferDescriptor descriptor{};
    descriptor.ByteWidth = element_count * sizeof(uint32_t);
    descriptor.Usage = D3D11_USAGE_IMMUTABLE;
    descriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
    dx::SubresourceDescriptor subresource_data{};
    subresource_data.pSysMem = data;
    return create_buffer(&descriptor, &subresource_data);
}

kl::dx::Buffer kl::DeviceHolder::create_index_buffer(const std::vector<uint32_t>& indices) const
{
    return create_index_buffer(indices.data(), (UINT) indices.size());
}

kl::dx::Buffer kl::DeviceHolder::create_const_buffer(const UINT byte_size) const
{
    if (!verify(byte_size % 16 == 0, "Constant buffer size has to be a multiple of 16")) {
        return {};
    }
    dx::BufferDescriptor descriptor{};
    descriptor.ByteWidth = byte_size;
    descriptor.Usage = D3D11_USAGE_DYNAMIC;
    descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    return create_buffer(&descriptor, nullptr);
}

kl::dx::Buffer kl::DeviceHolder::create_structured_buffer(const void* data, const UINT element_count, const UINT element_size, const bool has_unordered_access, const bool cpu_read) const
{
    dx::BufferDescriptor descriptor{};
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL);
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descriptor.CPUAccessFlags = cpu_read ? D3D11_CPU_ACCESS_READ : NULL;
    descriptor.StructureByteStride = element_size;
    descriptor.ByteWidth = element_count * element_size;
    dx::SubresourceDescriptor subresource{};
    subresource.pSysMem = data;
    return create_buffer(&descriptor, &subresource);
}

kl::dx::Buffer kl::DeviceHolder::create_staging_buffer(const dx::Buffer& buffer, const UINT byte_size) const
{
    dx::BufferDescriptor descriptor{};
    buffer->GetDesc(&descriptor);
    descriptor.Usage = D3D11_USAGE_STAGING;
    descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    descriptor.ByteWidth = byte_size ? byte_size : descriptor.ByteWidth;
    return create_buffer(&descriptor, nullptr);
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_screen_mesh()
{
    std::vector<Triangle> triangles;
    triangles.emplace_back(
        Vertex{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
        Vertex{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
        Vertex{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } }
    );
    triangles.emplace_back(
        Vertex{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
        Vertex{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
        Vertex{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } }
    );
    return triangles;
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_plane_mesh(const float size, const int complexity)
{
    assert(complexity >= 2, "Plane complexity must be at least 2");
    std::vector<Triangle> triangles;
    triangles.reserve(size_t(complexity - 1) * size_t(complexity - 1) * 2);
    const float incr = size / (complexity - 1);
    for (int x = 0; x < complexity - 1; x++) {
        for (int z = 0; z < complexity - 1; z++) {
            const Float3 lb = { x * incr, 0.0f, z * incr };
            const Float3 lt = lb + Float3{ 0.0f, 0.0f, incr };
			const Float3 rb = lb + Float3{ incr, 0.0f, 0.0f };
            const Float3 rt = lb + Float3{ incr, 0.0f, incr };
            triangles.emplace_back(
                Vertex{ lb, { 0.0f, 1.0f, 0.0f }, { float(x + 0) / (complexity - 1), 1.0f - float(z + 0) / (complexity - 1) } },
                Vertex{ lt, { 0.0f, 1.0f, 0.0f }, { float(x + 0) / (complexity - 1), 1.0f - float(z + 1) / (complexity - 1) } },
                Vertex{ rt, { 0.0f, 1.0f, 0.0f }, { float(x + 1) / (complexity - 1), 1.0f - float(z + 1) / (complexity - 1) } }
            );
            triangles.emplace_back(
                Vertex{ lb, { 0.0f, 1.0f, 0.0f }, { float(x + 0) / (complexity - 1), 1.0f - float(z + 0) / (complexity - 1) } },
                Vertex{ rt, { 0.0f, 1.0f, 0.0f }, { float(x + 1) / (complexity - 1), 1.0f - float(z + 1) / (complexity - 1) } },
                Vertex{ rb, { 0.0f, 1.0f, 0.0f }, { float(x + 1) / (complexity - 1), 1.0f - float(z + 0) / (complexity - 1) } }
            );
        }
    }
    return triangles;
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_cube_mesh(const float size)
{
    static constexpr Triangle face[2] = {
        Triangle{
            { { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
            { {  0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
            { {  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
        },
        Triangle{
            { { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
            { {  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
            { { -0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },
        },
    };
    static constexpr auto mul_vr = [](const Vertex& vertex, const Float3& other) -> Vertex
    {
        Vertex result;
        result.position = vertex.position * other;
        result.normal = vertex.normal;
        result.uv = vertex.uv;
        return result;
    };
    static constexpr auto rotate_vr = [](const Vertex& vertex, const Float3& axis, const float angle) -> Vertex
    {
        Vertex result;
        result.position = rotate(vertex.position, axis, angle);
        result.normal = rotate(vertex.normal, axis, angle);
        result.uv = vertex.uv;
        return result;
    };
    static constexpr auto mul_tr = [](const Triangle& triangle, const Float3& other) -> Triangle
    {
        Triangle result;
        result.a = mul_vr(triangle.a, other);
        result.b = mul_vr(triangle.b, other);
        result.c = mul_vr(triangle.c, other);
        return result;
    };
    static constexpr auto rotate_tr = [](const Triangle& triangle, const Float3& axis, const float angle) -> Triangle
    {
        Triangle result;
        result.a = rotate_vr(triangle.a, axis, angle);
        result.b = rotate_vr(triangle.b, axis, angle);
        result.c = rotate_vr(triangle.c, axis, angle);
        return result;
    };

    std::vector<Triangle> triangles;
    for (const auto& triangle : face) {
        triangles.push_back(mul_tr(triangle, Float3{ size }));
        triangles.push_back(rotate_tr(triangles.back(), { 1.0f, 0.0f, 0.0f }, 90.0f));
        triangles.push_back(rotate_tr(triangles.back(), { 0.0f, 1.0f, 0.0f }, 90.0f));
        triangles.push_back(rotate_tr(triangles.back(), { 0.0f, 1.0f, 0.0f }, 90.0f));
        triangles.push_back(rotate_tr(triangles.back(), { 0.0f, 1.0f, 0.0f }, 90.0f));
        triangles.push_back(rotate_tr(rotate_tr(triangles.back(), { 0.0f, 1.0f, 0.0f }, 90.0f), { 1.0f, 0.0f, 0.0f }, 90.0f));
    }
    return triangles;
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_sphere_mesh(const float radius, const int complexity, const bool smooth)
{
    static constexpr float X = 0.525731112119133606f;
    static constexpr float Z = 0.850650808352039932f;
    static constexpr float N = 0.0f;
    static constexpr Float3 vertices[12] = {
        { -X, N, Z }, {  X, N,  Z }, { -X,  N, -Z }, {  X,  N, -Z },
        {  N, Z, X }, {  N, Z, -X }, {  N, -Z,  X }, {  N, -Z, -X },
        {  Z, X, N }, { -Z, X,  N }, {  Z, -X,  N }, { -Z, -X,  N },
    };
    static constexpr Int3 indices[20] = {
        { 0,  4,  1 }, { 0, 9,  4 }, { 9,  5, 4 }, {  4, 5, 8 }, { 4, 8,  1 },
        { 8, 10,  1 }, { 8, 3, 10 }, { 5,  3, 8 }, {  5, 2, 3 }, { 2, 7,  3 },
        { 7, 10,  3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1,  6 },
        { 6,  1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, {  9, 2, 5 }, { 7, 2, 11 },
    };
    static constexpr auto subdivide_single = [](const Triangle& triangle, std::vector<Triangle>& triangles)
    {
        auto& a = triangle.a;
		auto& b = triangle.b;
		auto& c = triangle.c;
        const Float3 ab = normalize((triangle.a.position + triangle.b.position) * 0.5f);
        const Float3 bc = normalize((triangle.b.position + triangle.c.position) * 0.5f);
        const Float3 ca = normalize((triangle.c.position + triangle.a.position) * 0.5f);
        triangles.emplace_back(a, ab, ca);
        triangles.emplace_back(ab, b, bc);
        triangles.emplace_back(ca, bc, c);
        triangles.emplace_back(ab, bc, ca);
    };
    static constexpr auto subdivide_multiple = [](const std::vector<Triangle>& triangles) -> std::vector<Triangle>
    {
        std::vector<Triangle> result;
        for (const auto& triangle : triangles) {
            subdivide_single(triangle, result);
        }
        return result;
    };

    std::vector<Triangle> triangles;
    for (const auto& index : indices) {
        triangles.emplace_back(vertices[index.z], vertices[index.y], vertices[index.x]);
    }
    for (int i = 0; i < complexity; i++) {
        triangles = subdivide_multiple(triangles);
    }
    std::for_each(std::execution::par, triangles.begin(), triangles.end(), [radius, smooth](Triangle& triangle)
    {
        const Float3 a_norm = normalize(triangle.a.position);
		const Float3 b_norm = normalize(triangle.b.position);
		const Float3 c_norm = normalize(triangle.c.position);
        if (smooth) {
            triangle.a.normal = a_norm;
            triangle.b.normal = b_norm;
            triangle.c.normal = c_norm;
        }
        else {
            const Float3 normal = triangle.normal();
            triangle.a.normal = normal;
            triangle.b.normal = normal;
            triangle.c.normal = normal;
        }
        triangle.a.position = a_norm * radius;
        triangle.b.position = b_norm * radius;
        triangle.c.position = c_norm * radius;
    });
    return triangles;
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_capsule_mesh(const float radius, const float height, const int sectors, const int rings)
{
    const auto gen_hem = [&]
    {
        std::vector<Triangle> triangles;
        const float half_height = height * 0.5f;
        const Float3 top_center = Float3{ 0.0f, half_height, 0.0f };
        for (int i = 0; i < sectors; i++) {
            const float first_phi = (float(i) / sectors) * 2.0f * pi();
            const float first_x = cos(first_phi) * radius;
            const float first_z = sin(first_phi) * radius;
            const float second_phi = (float(i + 1) / sectors) * 2.0f * pi();
            const float second_x = cos(second_phi) * radius;
            const float second_z = sin(second_phi) * radius;
            const Float3 first = Float3{ first_x, half_height, first_z };
            const Float3 second = Float3{ second_x, half_height, second_z };
            Float3 first_dir = Float3{ 0.0f, half_height + radius, 0.0f } - first;
            Float3 second_dir = Float3{ 0.0f, half_height + radius, 0.0f } - second;
            float first_len = first_dir.length();
			float second_len = second_dir.length();
			first_dir /= first_len;
			second_dir /= second_len;
            first_len /= rings;
			second_len /= rings;
            for (int j = 0; j < rings; j++) {
                const Float3 left_first = normalize(first + first_dir * first_len * float(j) - top_center) * radius + top_center;
                const Float3 left_second = normalize(first + first_dir * first_len * float(j + 1) - top_center) * radius + top_center;
                const Float3 right_first = normalize(second + second_dir * second_len * float(j) - top_center) * radius + top_center;
				const Float3 right_second = normalize(second + second_dir * second_len * float(j + 1) - top_center) * radius + top_center;
                triangles.emplace_back(left_first, left_second, right_second);
                triangles.emplace_back(left_first, right_second, right_first);
            }
        }
        return triangles;
    };
    const auto gen_cyl = [&]
    {
        std::vector<Triangle> triangles;
        const float half_height = height * 0.5f;
        for (int i = 0; i < sectors; i++) {
            const float first_phi = (float(i) / sectors) * 2.0f * pi();
            const float first_x = cos(first_phi) * radius;
            const float first_z = sin(first_phi) * radius;
            const float second_phi = (float(i + 1) / sectors) * 2.0f * pi();
            const float second_x = cos(second_phi) * radius;
            const float second_z = sin(second_phi) * radius;
            triangles.emplace_back(
                Float3{  first_x, -half_height,  first_z },
                Float3{  first_x,  half_height,  first_z },
                Float3{ second_x,  half_height, second_z }
            );
            triangles.emplace_back(
                Float3{  first_x, -half_height,  first_z },
                Float3{ second_x,  half_height, second_z },
                Float3{ second_x, -half_height, second_z }
            );
        }
        return triangles;
    };

    auto top_hem = gen_hem();
    std::for_each(std::execution::par, top_hem.begin(), top_hem.end(), [](Triangle& triangle)
    {
        std::swap(triangle.a, triangle.c);
    });
    auto cylinder = gen_cyl();
    std::for_each(std::execution::par, cylinder.begin(), cylinder.end(), [](Triangle& triangle)
    {
        std::swap(triangle.a, triangle.c);
    });
    auto bottom_hem = gen_hem();
    std::for_each(std::execution::par, bottom_hem.begin(), bottom_hem.end(), [](Triangle& triangle)
    {
        triangle.a.position.y *= -1.0f;
        triangle.b.position.y *= -1.0f;
        triangle.c.position.y *= -1.0f;
    });

    std::vector<Triangle> triangles;
    triangles.insert(triangles.end(), top_hem.begin(), top_hem.end());
	triangles.insert(triangles.end(), cylinder.begin(), cylinder.end());
	triangles.insert(triangles.end(), bottom_hem.begin(), bottom_hem.end());
    std::for_each(std::execution::par, triangles.begin(), triangles.end(), [](Triangle& triangle)
    {
        std::swap(triangle.a.x, triangle.a.y);
        std::swap(triangle.b.x, triangle.b.y);
        std::swap(triangle.c.x, triangle.c.y);
        const Float3 normal = triangle.normal();
        triangle.a.normal = normal;
        triangle.b.normal = normal;
        triangle.c.normal = normal;
    });
    return triangles;
}

kl::dx::Buffer kl::DeviceHolder::create_screen_mesh() const
{
    return create_vertex_buffer(generate_screen_mesh());
}

kl::dx::Buffer kl::DeviceHolder::create_plane_mesh(const float size, const int complexity) const
{
    return create_vertex_buffer(generate_plane_mesh(size, complexity));
}

kl::dx::Buffer kl::DeviceHolder::create_cube_mesh(const float size) const
{
    return create_vertex_buffer(generate_cube_mesh(size));
}

kl::dx::Buffer kl::DeviceHolder::create_sphere_mesh(const float radius, const int complexity, const bool smooth) const
{
    return create_vertex_buffer(generate_sphere_mesh(radius, complexity, smooth));
}

kl::dx::Buffer kl::DeviceHolder::create_capsule_mesh(const float radius, const float height, const int sectors, const int rings) const
{
    return create_vertex_buffer(generate_capsule_mesh(radius, height, sectors, rings));
}

kl::dx::Texture kl::DeviceHolder::create_texture(const dx::TextureDescriptor* descriptor, const dx::SubresourceDescriptor* subresource_data) const
{
    dx::Texture texture;
    const long result = m_device->CreateTexture2D(descriptor, subresource_data, &texture);
    verify(texture, "Failed to create texture. Result: 0x", std::hex, result);
    return texture;
}

kl::dx::Texture kl::DeviceHolder::create_texture(const Image& image, const bool has_unordered_access, const bool is_target) const
{
    dx::TextureDescriptor descriptor{};
    descriptor.Width = image.width();
    descriptor.Height = image.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 1;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE |
        (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL) |
        (is_target ? D3D11_BIND_RENDER_TARGET : NULL);
    dx::SubresourceDescriptor data{};
    data.pSysMem = image.ptr();
    data.SysMemPitch = image.width() * sizeof(RGB);
    return create_texture(&descriptor, &data);
}

kl::dx::Texture kl::DeviceHolder::create_cube_texture(const Image& right, const Image& left, const Image& top, const Image& bottom, const Image& front, const Image& back) const
{
    if (!verify(right.size() == left.size()
        && right.size() == top.size()
        && right.size() == bottom.size()
        && right.size() == front.size()
        && right.size() == back.size(), "Sizes of the 6 given images do not match")) {
        return {};
    }
    dx::TextureDescriptor descriptor{};
    descriptor.Width = right.width();
    descriptor.Height = right.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 6;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    const UINT mem_pitch = UINT(right.width() * sizeof(RGB));
    const dx::SubresourceDescriptor data[6] = {
        { right.ptr(), mem_pitch, 0 },
        { left.ptr(), mem_pitch, 0 },
        { top.ptr(), mem_pitch, 0 },
        { bottom.ptr(), mem_pitch, 0 },
        { front.ptr(), mem_pitch, 0 },
        { back.ptr(), mem_pitch, 0 },
    };
    return create_texture(&descriptor, data);
}

kl::dx::Texture kl::DeviceHolder::create_staging_texture(const dx::Texture& texture, const Int2 size) const
{
    dx::TextureDescriptor descriptor{};
    texture->GetDesc(&descriptor);
    dx::TextureDescriptor staging_descriptor{};
    staging_descriptor.Width = (size.x > 0) ? size.x : descriptor.Width;
    staging_descriptor.Height = (size.y > 0) ? size.y : descriptor.Height;
    staging_descriptor.MipLevels = 1;
    staging_descriptor.ArraySize = 1;
    staging_descriptor.Format = descriptor.Format;
    staging_descriptor.SampleDesc.Count = 1;
    staging_descriptor.Usage = D3D11_USAGE_STAGING;
    staging_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    return create_texture(&staging_descriptor, nullptr);
}

kl::dx::Texture kl::DeviceHolder::create_target_texture(const Int2 size) const
{
    dx::TextureDescriptor descriptor{};
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    descriptor.Width = (UINT) size.x;
    descriptor.Height = (UINT) size.y;
    descriptor.ArraySize = 1;
    descriptor.MipLevels = 1;
    descriptor.SampleDesc.Count = 1;
    return create_texture(&descriptor, nullptr);
}

kl::dx::TargetView kl::DeviceHolder::create_target_view(const dx::Resource& resource, const dx::TargetViewDescriptor* descriptor) const
{
    dx::TargetView view;
    const long result = m_device->CreateRenderTargetView(resource.get(), descriptor, &view);
    verify(view, "Failed to create render target view. Result: 0x", std::hex, result);
    return view;
}

kl::dx::DepthView kl::DeviceHolder::create_depth_view(const dx::Resource& resource, const dx::DepthViewDescriptor* descriptor) const
{
    dx::DepthView view;
    const long result = m_device->CreateDepthStencilView(resource.get(), descriptor, &view);
    verify(view, "Failed to create depth view. Result: 0x", std::hex, result);
    return view;
}

kl::dx::ShaderView kl::DeviceHolder::create_shader_view(const dx::Resource& resource, const dx::ShaderViewDescriptor* descriptor) const
{
    dx::ShaderView view;
    const long result = m_device->CreateShaderResourceView(resource.get(), descriptor, &view);
    verify(view, "Failed to create shader view. Result: 0x", std::hex, result);
    return view;
}

kl::dx::AccessView kl::DeviceHolder::create_access_view(const dx::Resource& resource, const dx::AccessViewDescriptor* descriptor) const
{
    dx::AccessView view;
    const long result = m_device->CreateUnorderedAccessView(resource.get(), descriptor, &view);
    verify(view, "Failed to create unordered access view. Result: 0x", std::hex, result);
    return view;
}

kl::dx::InputLayout kl::DeviceHolder::create_input_layout(const CompiledShader& compiled_shader, const std::vector<dx::LayoutDescriptor>& descriptors) const
{
    static constexpr dx::LayoutDescriptor default_layout_descriptors[3] = {
        { "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "KL_UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    const dx::LayoutDescriptor* descriptors_ptr = !descriptors.empty() ? descriptors.data() : default_layout_descriptors;
    const UINT descriptors_count = !descriptors.empty() ? (UINT) descriptors.size() : (UINT) std::size(default_layout_descriptors);
    dx::InputLayout layout;
    m_device->CreateInputLayout(descriptors_ptr, descriptors_count, compiled_shader.data_ptr(), compiled_shader.data_size(), &layout);
    verify(layout, "Failed to create input layout");
    return layout;
}

kl::dx::VertexShader kl::DeviceHolder::create_vertex_shader(const CompiledShader& compiled_shader) const
{
    dx::VertexShader shader;
    m_device->CreateVertexShader(compiled_shader.data_ptr(), compiled_shader.data_size(), nullptr, &shader);
    verify(shader, "Failed to create vertex shader");
    return shader;
}

kl::dx::GeometryShader kl::DeviceHolder::create_geometry_shader(const CompiledShader& compiled_shader) const
{
    dx::GeometryShader shader;
    m_device->CreateGeometryShader(compiled_shader.data_ptr(), compiled_shader.data_size(), nullptr, &shader);
    verify(shader, "Failed to create geometry shader");
    return shader;
}

kl::dx::PixelShader kl::DeviceHolder::create_pixel_shader(const CompiledShader& compiled_shader) const
{
    dx::PixelShader shader;
    m_device->CreatePixelShader(compiled_shader.data_ptr(), compiled_shader.data_size(), nullptr, &shader);
    verify(shader, "Failed to create pixel shader");
    return shader;
}

kl::dx::ComputeShader kl::DeviceHolder::create_compute_shader(const CompiledShader& compiled_shader) const
{
    dx::ComputeShader shader;
    m_device->CreateComputeShader(compiled_shader.data_ptr(), compiled_shader.data_size(), nullptr, &shader);
    verify(shader, "Failed to create compute shader");
    return shader;
}
