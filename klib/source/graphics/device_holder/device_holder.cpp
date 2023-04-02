#include "graphics/device_holder/device_holder.h"

#include "utility/utility.h"
#include "memory/memory.h"


// Creation
kl::device_holder::device_holder()
{}

kl::device_holder::~device_holder()
{}

// States
kl::dx::raster_state kl::device_holder::create_raster_state(const dx::raster_state_descriptor* descriptor) const
{
    dx::raster_state state = nullptr;
    const long result = device_->CreateRasterizerState(descriptor, &state);
    warning_check(!state, format("Failed to create rasterizer state. Result: 0x", std::hex, result));
    return state;
}

kl::dx::raster_state kl::device_holder::create_raster_state(bool wireframe, bool cull, bool cull_back) const
{
    dx::raster_state_descriptor descriptor = {};
    descriptor.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    descriptor.CullMode = cull ? (cull_back ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
    descriptor.FrontCounterClockwise = true;
    descriptor.MultisampleEnable = true;
    descriptor.AntialiasedLineEnable = true;
    return create_raster_state(&descriptor);
}

kl::dx::depth_state kl::device_holder::create_depth_state(const dx::depth_state_descriptor* descriptor) const
{
    dx::depth_state state = nullptr;
    const long result = device_->CreateDepthStencilState(descriptor, &state);
    warning_check(!state, format("Failed to create depth stencil state. Result: 0x", std::hex, result));
    return state;
}

kl::dx::depth_state kl::device_holder::create_depth_state(bool depth, bool stencil, bool mask) const
{
    dx::depth_state_descriptor descriptor = {};
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

kl::dx::sampler_state kl::device_holder::create_sampler_state(const dx::sampler_state_descriptor* descriptor) const
{
    dx::sampler_state state = nullptr;
    const long result = device_->CreateSamplerState(descriptor, &state);
    warning_check(!state, format("Failed to create sampler state. Result: 0x", std::hex, result));
    return state;
}

kl::dx::sampler_state kl::device_holder::create_sampler_state(bool linear, bool mirror) const
{
    dx::sampler_state_descriptor descriptor = {};
    descriptor.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    descriptor.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    descriptor.AddressV = descriptor.AddressU;
    descriptor.AddressW = descriptor.AddressU;
    return create_sampler_state(&descriptor);
}

// Buffers
kl::dx::buffer kl::device_holder::create_buffer(const dx::buffer_descriptor* descriptor, const dx::subresource_descriptor* subresource_data) const
{
    dx::buffer buffer = nullptr;
    const long result = device_->CreateBuffer(descriptor, subresource_data, &buffer);
    warning_check(!buffer, format("Failed to create gpu buffer. Result: 0x", std::hex, result));
    return buffer;
}

kl::dx::buffer kl::device_holder::create_structured_buffer(const void* data, const UINT element_count, const UINT element_size, const bool has_unordered_access, const bool cpu_read) const
{
    dx::buffer_descriptor descriptor = {};
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL);
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descriptor.CPUAccessFlags = cpu_read ? D3D11_CPU_ACCESS_READ : NULL;
    descriptor.StructureByteStride = element_size;
    descriptor.ByteWidth = element_count * element_size;

    dx::subresource_descriptor subresource = {};
    subresource.pSysMem = data;

    return create_buffer(&descriptor, &subresource);
}

kl::dx::buffer kl::device_holder::create_staging_buffer(dx::buffer buffer, const UINT byte_size) const
{
    dx::buffer_descriptor descriptor = {};
    buffer->GetDesc(&descriptor);

    dx::buffer_descriptor staging_descriptor = {};
    staging_descriptor.Usage = D3D11_USAGE_STAGING;
    staging_descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    staging_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    staging_descriptor.StructureByteStride = descriptor.StructureByteStride;
    staging_descriptor.ByteWidth = byte_size ? byte_size : descriptor.ByteWidth;

    return create_buffer(&descriptor, nullptr);
}

// Const buffers
kl::dx::buffer kl::device_holder::create_const_buffer(const UINT byte_size) const
{
    if (warning_check(byte_size % 16, "Constant buffer size has to be a multiple of 16")) {
        return nullptr;
    }

    dx::buffer_descriptor descriptor = {};
    descriptor.ByteWidth = byte_size;
    descriptor.Usage = D3D11_USAGE_DYNAMIC;
    descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    return create_buffer(&descriptor, nullptr);
}

// Meshes
kl::dx::buffer kl::device_holder::create_mesh(const mesh_data& vertices) const
{
    dx::buffer_descriptor descriptor = {};
    descriptor.ByteWidth = (UINT) (vertices.size() * sizeof(vertex));
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    dx::subresource_descriptor subresource_data = {};
    subresource_data.pSysMem = vertices.data();

    return create_buffer(&descriptor, &subresource_data);
}

kl::dx::buffer kl::device_holder::create_mesh(const std::string& filepath, bool flip_z) const
{
    return create_mesh(files::parse_mesh(filepath, flip_z));
}

kl::dx::buffer kl::device_holder::create_plane_mesh(float size, size_t num_of_points) const
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
    for (size_t x = 0; x < num_of_points - 1; x++) {
        for (size_t z = 0; z < num_of_points - 1; z++) {
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

    return create_mesh(vertices);
}

kl::dx::buffer kl::device_holder::create_screen_mesh() const
{
    return create_mesh({
        vertex({ -1.0f, -1.0f, 0.5f }), vertex({ -1.0f,  1.0f, 0.5f }), vertex({  1.0f,  1.0f, 0.5f }),
        vertex({  1.0f,  1.0f, 0.5f }), vertex({  1.0f, -1.0f, 0.5f }), vertex({ -1.0f, -1.0f, 0.5f }),
    });
}

// Textures
kl::dx::texture kl::device_holder::create_texture(const dx::texture_descriptor* descriptor, const dx::subresource_descriptor* subresource_data) const
{
    dx::texture texture = nullptr;
    const long result = device_->CreateTexture2D(descriptor, subresource_data, &texture);
    warning_check(!texture, format("Failed to create texture. Result: 0x", std::hex, result));
    return texture;
}

kl::dx::texture kl::device_holder::create_texture(const image& image, bool has_unordered_access, bool is_target) const
{
    const kl::image flipped_image = image.flip_vertical();

    dx::texture_descriptor descriptor = {};
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

    dx::subresource_descriptor data = {};
    data.pSysMem = flipped_image;
    data.SysMemPitch = image.width() * sizeof(color);

    return create_texture(&descriptor, &data);
}

kl::dx::texture kl::device_holder::create_cube_texture(const image& front, const image& back, const image& left, const image& right, const image& top, const image& bottom) const
{
    if (warning_check((front.size() != back.size()) || (front.size() != left.size()) || (front.size() != right.size()) || (front.size() != top.size()) || (front.size() != bottom.size()), "Sizes of the 6 given images do not match")) {
        return nullptr;
    }

    dx::texture_descriptor descriptor = {};
    descriptor.Width = front.width();
    descriptor.Height = front.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 6;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    const UINT mem_pitch = (UINT) (front.width() * sizeof(color));
    const dx::subresource_descriptor data[6] = {
        { right,  mem_pitch, 0 },
        { left,   mem_pitch, 0 },
        { top,    mem_pitch, 0 },
        { bottom, mem_pitch, 0 },
        { front,  mem_pitch, 0 },
        { back,   mem_pitch, 0 },
    };

    return create_texture(&descriptor, data);
}

kl::dx::texture kl::device_holder::create_staging_texture(dx::texture texture, const int2& size) const
{
    dx::texture_descriptor descriptor = {};
    texture->GetDesc(&descriptor);

    dx::texture_descriptor staging_descriptor = {};
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

// Views
kl::dx::target_view kl::device_holder::create_target_view(const dx::resource resource, const dx::target_view_descriptor* descriptor) const
{
    dx::target_view view = nullptr;
    const long result = device_->CreateRenderTargetView(resource.Get(), descriptor, &view);
    warning_check(!view, format("Failed to create render target view. Result: 0x", std::hex, result));
    return view;
}

kl::dx::depth_view kl::device_holder::create_depth_view(const dx::resource resource, const dx::depth_view_descriptor* descriptor) const
{
    dx::depth_view view = nullptr;
    const long result = device_->CreateDepthStencilView(resource.Get(), descriptor, &view);
    warning_check(!view, format("Failed to create depth view. Result: 0x", std::hex, result));
    return view;
}

kl::dx::shader_view kl::device_holder::create_shader_view(const dx::resource resource, const dx::shader_view_descriptor* descriptor) const
{
    dx::shader_view view = nullptr;
    const long result = device_->CreateShaderResourceView(resource.Get(), descriptor, &view);
    warning_check(!view, format("Failed to create shader view. Result: 0x", std::hex, result));
    return view;
}

kl::dx::access_view kl::device_holder::create_access_view(const dx::resource resource, const dx::access_view_descriptor* descriptor) const
{
    dx::access_view view = nullptr;
    const long result = device_->CreateUnorderedAccessView(resource.Get(), descriptor, &view);
    warning_check(!view, format("Failed to create unordered access view. Result: 0x", std::hex, result));
    return view;
}

// Shaders
kl::dx::layout kl::device_holder::create_input_layout(const compiled_shader& compiled_shader, const std::vector<dx::layout_descriptor>& descriptors) const
{
    static const dx::layout_descriptor default_layout_descriptors[3] = {
        { "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "KL_Texture", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    const dx::layout_descriptor* descriptors_ptr = (!descriptors.empty()) ? descriptors.data() : default_layout_descriptors;
    const UINT descriptors_count = (!descriptors.empty()) ? ((UINT) descriptors.size()) : 3;

    dx::layout layout = nullptr;
    device_->CreateInputLayout(descriptors_ptr, descriptors_count, compiled_shader.get_data(), compiled_shader.get_data_size(), &layout);
    warning_check(!layout, "Failed to create input layout");
    return layout;
}

kl::dx::vertex_shader kl::device_holder::create_vertex_shader(const compiled_shader& compiled_shader) const
{
    dx::vertex_shader shader = nullptr;
    device_->CreateVertexShader(compiled_shader.get_data(), compiled_shader.get_data_size(), nullptr, &shader);
    warning_check(!shader, "Failed to create vertex shader");
    return shader;
}

kl::dx::geometry_shader kl::device_holder::create_geometry_shader(const compiled_shader& compiled_shader) const
{
    dx::geometry_shader shader = nullptr;
    device_->CreateGeometryShader(compiled_shader.get_data(), compiled_shader.get_data_size(), nullptr, &shader);
    warning_check(!shader, "Failed to create geometry shader");
    return shader;
}

kl::dx::pixel_shader kl::device_holder::create_pixel_shader(const compiled_shader& compiled_shader) const
{
    dx::pixel_shader shader = nullptr;
    device_->CreatePixelShader(compiled_shader.get_data(), compiled_shader.get_data_size(), nullptr, &shader);
    warning_check(!shader, "Failed to create pixel shader");
    return shader;
}

kl::dx::compute_shader kl::device_holder::create_compute_shader(const compiled_shader& compiled_shader) const
{
    dx::compute_shader shader = nullptr;
    device_->CreateComputeShader(compiled_shader.get_data(), compiled_shader.get_data_size(), nullptr, &shader);
    warning_check(!shader, "Failed to create compute shader");
    return shader;
}

kl::render_shaders kl::device_holder::create_render_shaders(const compiled_shader& compiled_vertex_shader, const compiled_shader& compiled_pixel_shader) const
{
    render_shaders shaders = {};
    shaders.input_layout = create_input_layout(compiled_vertex_shader);
    shaders.vertex_shader = create_vertex_shader(compiled_vertex_shader);
    shaders.pixel_shader = create_pixel_shader(compiled_pixel_shader);
    return shaders;
}
