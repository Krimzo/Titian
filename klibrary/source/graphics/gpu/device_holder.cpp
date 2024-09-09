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
    descriptor.FrontCounterClockwise = true;
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

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(const std::vector<Vertex<float>>& vertices) const
{
    return create_vertex_buffer(vertices.data(), (UINT) (vertices.size() * sizeof(Vertex<float>)));
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(const std::string_view& filepath) const
{
    const auto data = parse_obj_file(filepath, true);
    return create_vertex_buffer(data);
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

kl::dx::Buffer kl::DeviceHolder::create_plane_mesh(const float size, size_t num_of_points) const
{
    const float increment = size / (float) num_of_points;
    num_of_points += 1;

    std::vector<Float2> points{};
    points.reserve(num_of_points * num_of_points);
    for (int x = 0; x < num_of_points; x++) {
        for (int z = 0; z < num_of_points; z++) {
            points.emplace_back(
                -size * 0.5f + increment * x,
                -size * 0.5f + increment * z
            );
        }
    }

    std::vector<Vertex<float>> vertices{};
    vertices.reserve((num_of_points - 1) * (num_of_points - 1) * 6);
    for (size_t x = 0; x < num_of_points - 1; x++) {
        for (size_t z = 0; z < num_of_points - 1; z++) {
            const Float2& xz_point = points[z * num_of_points + x];
            const Float2& xz_plus_point = points[(z + 1) * num_of_points + (x + 1)];

            vertices.push_back(Vertex({ xz_plus_point.x, 0, xz_plus_point.y }));
            vertices.push_back(Vertex({ xz_plus_point.x, 0, xz_point.y }));
            vertices.push_back(Vertex({ xz_point.x, 0, xz_point.y }));

            vertices.push_back(Vertex({ xz_point.x, 0, xz_point.y }));
            vertices.push_back(Vertex({ xz_point.x, 0, xz_plus_point.y }));
            vertices.push_back(Vertex({ xz_plus_point.x, 0, xz_plus_point.y }));
        }
    }

    return create_vertex_buffer(vertices);
}

kl::dx::Buffer kl::DeviceHolder::create_screen_mesh() const
{
    return create_vertex_buffer({
        Vertex({ -1.0f, 1.0f, 0.5f }, { 0.0f, 0.0f }), Vertex({ 1.0f,  1.0f, 0.5f }, { 1.0f, 0.0f }), Vertex({  1.0f, -1.0f, 0.5f }, { 1.0f, 1.0f }),
        Vertex({ -1.0f, 1.0f, 0.5f }, { 0.0f, 0.0f }), Vertex({ 1.0f, -1.0f, 0.5f }, { 1.0f, 1.0f }), Vertex({ -1.0f, -1.0f, 0.5f }, { 0.0f, 1.0f }),
    });
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
    data.SysMemPitch = image.width() * sizeof(Color);

    return create_texture(&descriptor, &data);
}

kl::dx::Texture kl::DeviceHolder::create_cube_texture(const Image& front, const Image& back, const Image& left, const Image& right, const Image& top, const Image& bottom) const
{
    if (!verify(front.size() == back.size()
        && front.size() == left.size()
        && front.size() == right.size()
        && front.size() == top.size()
        && front.size() == bottom.size(), "Sizes of the 6 given images do not match")) {
        return {};
    }

    dx::TextureDescriptor descriptor{};
    descriptor.Width = front.width();
    descriptor.Height = front.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 6;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    const UINT mem_pitch = (UINT) (front.width() * sizeof(Color));
    const dx::SubresourceDescriptor data[6] = {
        { right.ptr(),  mem_pitch, 0 },
        { left.ptr(),   mem_pitch, 0 },
        { top.ptr(),    mem_pitch, 0 },
        { bottom.ptr(), mem_pitch, 0 },
        { front.ptr(),  mem_pitch, 0 },
        { back.ptr(),   mem_pitch, 0 },
    };
    return create_texture(&descriptor, data);
}

kl::dx::Texture kl::DeviceHolder::create_staging_texture(const dx::Texture& texture, const Int2& size) const
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

kl::dx::Texture kl::DeviceHolder::create_target_texture(const kl::Int2& size) const
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
        { "KL_Texture", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    const dx::LayoutDescriptor* descriptors_ptr = (!descriptors.empty()) ? descriptors.data() : default_layout_descriptors;
    const UINT descriptors_count = (!descriptors.empty()) ? ((UINT) descriptors.size()) : ((UINT) std::size(default_layout_descriptors));

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
