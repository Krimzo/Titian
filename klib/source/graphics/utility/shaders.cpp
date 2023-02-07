#include "graphics/gpu.h"

#include "utility/console.h"


struct shader_blobs
{
    ID3DBlob* data = nullptr;
    ID3DBlob* error = nullptr;

    shader_blobs() = default;

    shader_blobs(const shader_blobs&) = delete;
    shader_blobs(const shader_blobs&&) = delete;

    void operator=(const shader_blobs&) = delete;
    void operator=(const shader_blobs&&) = delete;

    ~shader_blobs()
    {
        if (data) data->Release();
        if (error) error->Release();
    }

    [[nodiscard]] bool has_data() const
    {
        return data;
    }

    [[nodiscard]] void* data_pointer() const
    {
        if (data) {
            return data->GetBufferPointer();
        }
        return nullptr;
    }

    [[nodiscard]] SIZE_T data_size() const
    {
        if (data) {
            return data->GetBufferSize();
        }
        return 0;
    }

    [[nodiscard]] std::string get_error() const
    {
        if (error) {
            return (char*) error->GetBufferPointer();
        }
        return "Unknown";
    }
};

kl::dx::vertex_shader kl::gpu::new_vertex_shader(const std::string& source, dx::layout* out_layout, const std::vector<dx::layout_descriptor>& descriptors)
{
    shader_blobs blobs = {};

    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "v_shader", "vs_5_0", NULL, NULL, &blobs.data, &blobs.error);
    if (warning_check(!blobs.has_data(), "Failed to compile vertex shader. Error: " + blobs.get_error())) {
        return nullptr;
    }

    dx::vertex_shader shader = nullptr;
    device_->CreateVertexShader(blobs.data_pointer(), blobs.data_size(), nullptr, &shader);
    if (warning_check(!shader, "Failed to create vertex shader")) {
        return nullptr;
    }

    if (out_layout) {
        if (!descriptors.empty()) {
            device_->CreateInputLayout(descriptors.data(), UINT(descriptors.size()), blobs.data_pointer(), blobs.data_size(), out_layout);
        }
        else {
            constexpr dx::layout_descriptor default_layout_descriptors[3] = {
                {
                    "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                    D3D11_INPUT_PER_VERTEX_DATA, 0
                },
                {
                    "KL_Texture", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                    D3D11_INPUT_PER_VERTEX_DATA, 0
                },
                {
                    "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                    D3D11_INPUT_PER_VERTEX_DATA, 0
                }
            };

            device_->CreateInputLayout(default_layout_descriptors, 3, blobs.data_pointer(), blobs.data_size(), out_layout);
        }
        if (!warning_check(!*out_layout, "Failed to create input layout")) {
            children_.insert(*out_layout);
        }
    }

    children_.insert(shader);

    return shader;
}

kl::dx::pixel_shader kl::gpu::new_pixel_shader(const std::string& source)
{
    shader_blobs blobs = {};

    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "p_shader", "ps_5_0", NULL, NULL, &blobs.data, &blobs.error);
    if (warning_check(!blobs.has_data(), "Failed to compile pixel shader. Error: " + blobs.get_error())) {
        return nullptr;
    }

    dx::pixel_shader shader = nullptr;
    device_->CreatePixelShader(blobs.data_pointer(), blobs.data_size(), nullptr, &shader);
    if (warning_check(!shader, "Failed to create pixel shader")) {
        return nullptr;
    }

    children_.insert(shader);

    return shader;
}

kl::dx::geometry_shader kl::gpu::new_geometry_shader(const std::string& source)
{
    shader_blobs blobs = {};

    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "g_shader", "gs_5_0", NULL, NULL, &blobs.data, &blobs.error);
    if (warning_check(!blobs.has_data(), "Failed to compile geometry shader. Error: " + blobs.get_error())) {
        return nullptr;
    }

    dx::geometry_shader shader = nullptr;
    device_->CreateGeometryShader(blobs.data_pointer(), blobs.data_size(), nullptr, &shader);
    if (warning_check(!shader, "Failed to create geometry shader")) {
        return nullptr;
    }

    children_.insert(shader);

    return shader;
}

kl::dx::compute_shader kl::gpu::new_compute_shader(const std::string& source)
{
    shader_blobs blobs = {};

    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "c_shader", "cs_5_0", NULL, NULL, &blobs.data, &blobs.error);
    if (warning_check(!blobs.has_data(), "Failed to compile compute shader. Error: " + blobs.get_error())) {
        return nullptr;
    }

    dx::compute_shader shader = nullptr;
    device_->CreateComputeShader(blobs.data_pointer(), blobs.data_size(), nullptr, &shader);
    if (warning_check(!shader, "Failed to create compute shader")) {
        return nullptr;
    }

    children_.insert(shader);

    return shader;
}

kl::shaders kl::gpu::new_shaders(const std::string& vertex_source, const std::string& pixel_source, const std::vector<dx::layout_descriptor>& descriptors)
{
    dx::layout layout = nullptr;
    const dx::vertex_shader vertex_shader = new_vertex_shader(vertex_source, &layout, descriptors);
    return { vertex_shader, new_pixel_shader(pixel_source), layout };
}

kl::shaders kl::gpu::new_shaders(const std::string& source, const std::vector<dx::layout_descriptor>& descriptors)
{
    return new_shaders(source, source, descriptors);
}

void kl::gpu::bind_vertex_shader(const dx::vertex_shader shader) const
{
    context_->VSSetShader(shader, nullptr, 0);
}

void kl::gpu::bind_pixel_shader(const dx::pixel_shader shader) const
{
    context_->PSSetShader(shader, nullptr, 0);
}

void kl::gpu::bind_geometry_shader(const dx::geometry_shader shader) const
{
    context_->GSSetShader(shader, nullptr, 0);
}

void kl::gpu::bind_compute_shader(const dx::compute_shader shader) const
{
    context_->CSSetShader(shader, nullptr, 0);
}

void kl::gpu::bind_layout(const dx::layout layout) const
{
    context_->IASetInputLayout(layout);
}

void kl::gpu::bind_shaders(const shaders& shaders) const
{
    bind_vertex_shader(shaders.vertex_shader);
    bind_pixel_shader(shaders.pixel_shader);
    bind_layout(shaders.layout);
}

void kl::gpu::dispatch_compute_shader(const int3& size) const
{
    context_->Dispatch(size.x, size.y, size.z);
}

void kl::gpu::execute_compute_shader(const dx::compute_shader shader, const int3& size) const
{
    bind_compute_shader(shader);
    dispatch_compute_shader(size);
}

void kl::gpu::destroy(const shaders& shaders)
{
    destroy(shaders.vertex_shader);
    destroy(shaders.pixel_shader);
    destroy(shaders.layout);
}
