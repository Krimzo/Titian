#include "main.h"


titian::Material::Material()
{}

void titian::Material::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_object<float>(alpha_blend);
	serializer->write_object<float>(texture_blend);

	serializer->write_object<float>(reflection_factor);
	serializer->write_object<float>(refraction_factor);
	serializer->write_object<float>(refraction_index);

	serializer->write_object<kl::Float4>(color);

	serializer->write_string(color_map_name);
	serializer->write_string(normal_map_name);
	serializer->write_string(roughness_map_name);

	serializer->write_string(custom_shader_name);
}

void titian::Material::deserialize(const Serializer* serializer, const void* helper_data)
{
	serializer->read_object<float>(alpha_blend);
	serializer->read_object<float>(texture_blend);

	serializer->read_object<float>(reflection_factor);
	serializer->read_object<float>(refraction_factor);
	serializer->read_object<float>(refraction_index);

	serializer->read_object<kl::Float4>(color);

	serializer->read_string(color_map_name);
	serializer->read_string(normal_map_name);
	serializer->read_string(roughness_map_name);

	serializer->read_string(custom_shader_name);
}

bool titian::Material::is_transparent() const
{
	return alpha_blend < 1.0f;
}
