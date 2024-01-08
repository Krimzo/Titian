#include "main.h"


titian::Material::Material(kl::GPU* gpu)
	: m_gpu(gpu)
{}

void titian::Material::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_object<float>(texture_blend);
	serializer->write_object<float>(reflection_factor);
	serializer->write_object<float>(refraction_factor);
	serializer->write_object<float>(refraction_index);

	serializer->write_object<kl::Float4>(color);

	serializer->write_string(color_map_name);
	serializer->write_string(normal_map_name);
	serializer->write_string(roughness_map_name);

	serializer->write_string(shader_source);
	serializer->write_string(shader_source_file);
}

void titian::Material::deserialize(const Serializer* serializer, const void* helper_data)
{
	serializer->read_object<float>(texture_blend);
	serializer->read_object<float>(reflection_factor);
	serializer->read_object<float>(refraction_factor);
	serializer->read_object<float>(refraction_index);

	serializer->read_object<kl::Float4>(color);

	serializer->read_string(color_map_name);
	serializer->read_string(normal_map_name);
	serializer->read_string(roughness_map_name);

	serializer->read_string(shader_source);
	serializer->read_string(shader_source_file);

	this->reload();
}

void titian::Material::reload()
{
	if (std::filesystem::exists(shader_source_file)) {
		shader_source = kl::read_file_string(shader_source_file);
	}
	if (!shader_source.empty()) {
		shaders = m_gpu->create_render_shaders(shader_source);
	}
}
