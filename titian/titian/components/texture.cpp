#include "main.h"


titian::Texture::Texture(kl::GPU* gpu)
	: m_gpu(gpu)
{}

void titian::Texture::serialize(Serializer* serializer, const void* helper_data) const
{
	Vector<byte> compressed_image;
	data_buffer.save_to_vector(&compressed_image, COMPRESSION_TYPE);

	serializer->write_object<uint64_t>(compressed_image.size());
	serializer->write_array<byte>(compressed_image.data(), compressed_image.size());
	serializer->write_object<bool>(m_is_cube);
}

void titian::Texture::deserialize(const Serializer* serializer, const void* helper_data)
{
	const uint64_t data_size = serializer->read_object<uint64_t>();
	Vector<byte> image_data{};
	image_data.resize(data_size);

	serializer->read_array<byte>(image_data.data(), data_size);
	data_buffer.load_from_vector(image_data);

	serializer->read_object<bool>(m_is_cube);
	if (m_is_cube) {
		reload_as_cube();
	}
	else {
		reload_as_2D(false, false);
	}
	create_shader_view(nullptr);
}

void titian::Texture::reload_as_2D(bool has_unordered_access, bool is_target)
{
	graphics_buffer = m_gpu->create_texture(data_buffer, has_unordered_access, is_target);
	m_is_cube = false;
}

bool titian::Texture::reload_as_cube()
{
	if (data_buffer.width() % 4 || data_buffer.height() % 3) {
		return false;
	}

	const int part_size = data_buffer.width() / 4;
	const kl::Image box_sides[6] = {
		data_buffer.rectangle(kl::Int2(1, 1) * part_size, kl::Int2(2, 2) * part_size),
		data_buffer.rectangle(kl::Int2(3, 1) * part_size, kl::Int2(4, 2) * part_size),
		data_buffer.rectangle(kl::Int2(0, 1) * part_size, kl::Int2(1, 2) * part_size),
		data_buffer.rectangle(kl::Int2(2, 1) * part_size, kl::Int2(3, 2) * part_size),
		data_buffer.rectangle(kl::Int2(1, 0) * part_size, kl::Int2(2, 1) * part_size),
		data_buffer.rectangle(kl::Int2(1, 2) * part_size, kl::Int2(2, 3) * part_size),
	};

	graphics_buffer = m_gpu->create_cube_texture(box_sides[0], box_sides[1], box_sides[2], box_sides[3], box_sides[4], box_sides[5]);
	m_is_cube = true;
	return true;
}

void titian::Texture::create_target_view(const kl::dx::TargetViewDescriptor* descriptor)
{
	target_view = m_gpu->create_target_view(graphics_buffer, descriptor);
}

void titian::Texture::create_depth_view(const kl::dx::DepthViewDescriptor* descriptor)
{
	depth_view = m_gpu->create_depth_view(graphics_buffer, descriptor);
}

void titian::Texture::create_shader_view(const kl::dx::ShaderViewDescriptor* descriptor)
{
	shader_view = m_gpu->create_shader_view(graphics_buffer, descriptor);
}

void titian::Texture::create_access_view(const kl::dx::AccessViewDescriptor* descriptor)
{
	access_view = m_gpu->create_access_view(graphics_buffer, descriptor);
}

bool titian::Texture::is_cube() const
{
	return m_is_cube;
}

kl::Int2 titian::Texture::graphics_buffer_size() const
{
	if (!graphics_buffer) {
		return {};
	}

	kl::dx::TextureDescriptor descriptor = {};
	graphics_buffer->GetDesc(&descriptor);
	return {
		static_cast<int>(descriptor.Width),
		static_cast<int>(descriptor.Height),
	};
}
