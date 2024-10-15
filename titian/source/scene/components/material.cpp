#include "titian.h"


titian::Material::Material()
{}

void titian::Material::serialize(Serializer& serializer) const
{
	serializer.write_float("texture_blend", texture_blend);
	serializer.write_float("reflectivity_factor", reflectivity_factor);
	serializer.write_float("refraction_index", refraction_index);

	serializer.write_float_array("color", &color.x, 4);
	serializer.write_float_array("custom_data", custom_data.data, 16);

	serializer.write_string("color_texture_name", color_texture_name);
	serializer.write_string("normal_texture_name", normal_texture_name);
	serializer.write_string("roughness_texture_name", roughness_texture_name);
	serializer.write_string("shader_name", shader_name);
}

void titian::Material::deserialize(const Serializer& serializer)
{
	serializer.read_float("texture_blend", texture_blend);
	serializer.read_float("reflectivity_factor", reflectivity_factor);
	serializer.read_float("refraction_index", refraction_index);

	serializer.read_float_array("color", &color.x, 4);
	serializer.read_float_array("custom_data", custom_data.data, 16);

	serializer.read_string("color_texture_name", color_texture_name);
	serializer.read_string("normal_texture_name", normal_texture_name);
	serializer.read_string("roughness_texture_name", roughness_texture_name);
	serializer.read_string("shader_name", shader_name);
}

bool titian::Material::is_transparent() const
{
	return color.w < 1.0f;
}

titian::Ref<titian::Material> titian::Material::clone() const
{
	Ref material = new Material();
	material->texture_blend = texture_blend;
	material->reflectivity_factor = reflectivity_factor;
	material->refraction_index = refraction_index;
	material->color = color;
	material->custom_data = custom_data;
	material->color_texture_name = color_texture_name;
	material->normal_texture_name = normal_texture_name;
	material->roughness_texture_name = roughness_texture_name;
	material->shader_name = shader_name;
	return material;
}
