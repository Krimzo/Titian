#include "fuze.h"


titian::Frame::Frame()
{}

void titian::Frame::upload(const Image& image)
{
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
	gpu->write_to_texture(m_staging_texture, image, image.size(), sizeof(Color), false);
	gpu->copy_resource(texture, m_staging_texture);
}

void titian::Frame::retrieve(Image& image) const
{
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    gpu->copy_resource(m_staging_texture, texture);
    gpu->read_from_texture(image, m_staging_texture, image.size(), sizeof(Color));
}

titian::Int2 titian::Frame::size() const
{
	return m_size;
}

void titian::Frame::resize(const Int2& size)
{
    if (size.x <= 0 || size.y <= 0 || m_size == size)
        return;

    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

	dx::TextureDescriptor desc{};
    desc.Width = (UINT) size.x;
    desc.Height = (UINT) size.y;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    desc.CPUAccessFlags = NULL;
	texture = gpu->create_texture(&desc, nullptr);
    shader_view = gpu->create_shader_view(texture, nullptr);
    access_view = gpu->create_access_view(texture, nullptr);

    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = NULL;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	m_staging_texture = gpu->create_texture(&desc, nullptr);

    m_size = size;
}
