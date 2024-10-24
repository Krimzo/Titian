#include "titian.h"


titian::Texture::Texture()
{}

void titian::Texture::serialize(Serializer& serializer) const
{
	String compressed_data;
	image.save_to_buffer(compressed_data, COMPRESSION_TYPE);

	serializer.write_int("compressed_size", (int32_t) compressed_data.size());
	serializer.write_byte_array("compressed_data", compressed_data.data(), (int32_t) compressed_data.size());
	serializer.write_bool("is_cube", m_is_cube);
}

void titian::Texture::deserialize(const Serializer& serializer)
{
	int32_t compressed_size = 0;
	serializer.read_int("compressed_size", compressed_size);

	String compressed_data;
	compressed_data.resize(compressed_size);
	serializer.read_byte_array("compressed_data", compressed_data.data(), (int32_t) compressed_data.size());
	image.load_from_buffer(compressed_data);

	serializer.read_bool("is_cube", m_is_cube);
	if (m_is_cube) {
		reload_as_cube();
	}
	else {
		reload_as_2D();
	}
	create_shader_view(nullptr);
}

void titian::Texture::reload_as_2D()
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	texture = gpu.create_texture(image);
	m_is_cube = false;
}

bool titian::Texture::reload_as_cube()
{
	if (image.width() % 4 || image.height() % 3) {
		return false;
	}

	const int part_size = image.width() / 4;
	const kl::Image box_sides[6] = {
		image.rectangle(Int2(1, 1) * part_size, Int2(2, 2) * part_size),
		image.rectangle(Int2(3, 1) * part_size, Int2(4, 2) * part_size),
		image.rectangle(Int2(0, 1) * part_size, Int2(1, 2) * part_size),
		image.rectangle(Int2(2, 1) * part_size, Int2(3, 2) * part_size),
		image.rectangle(Int2(1, 0) * part_size, Int2(2, 1) * part_size),
		image.rectangle(Int2(1, 2) * part_size, Int2(2, 3) * part_size),
	};

	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	texture = gpu.create_cube_texture(box_sides[0], box_sides[1], box_sides[2], box_sides[3], box_sides[4], box_sides[5]);
	m_is_cube = true;
	return true;
}

void titian::Texture::create_target_view(const dx::TargetViewDescriptor* descriptor)
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	target_view = gpu.create_target_view(texture, descriptor);
}

void titian::Texture::create_depth_view(const dx::DepthViewDescriptor* descriptor)
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	depth_view = gpu.create_depth_view(texture, descriptor);
}

void titian::Texture::create_shader_view(const dx::ShaderViewDescriptor* descriptor)
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	shader_view = gpu.create_shader_view(texture, descriptor);
}

void titian::Texture::create_access_view(const dx::AccessViewDescriptor* descriptor)
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	access_view = gpu.create_access_view(texture, descriptor);
}

bool titian::Texture::is_cube() const
{
	return m_is_cube;
}

titian::Int2 titian::Texture::resolution() const
{
	if (!texture) {
		return {};
	}
	dx::TextureDescriptor descriptor{};
	texture->GetDesc(&descriptor);
	return { int(descriptor.Width), int(descriptor.Height) };
}

void titian::Texture::copy_other(const dx::Texture& texture)
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	if (gpu.texture_size(this->texture) != gpu.texture_size(texture)) {
		dx::TextureDescriptor descriptor{};
		texture->GetDesc(&descriptor);
		this->texture = gpu.create_texture(&descriptor, nullptr);
		create_shader_view(nullptr);
	}
	gpu.copy_resource(this->texture, texture);
}
