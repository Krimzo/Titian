#include "main.h"


titian::Material::Material(kl::GPU* gpu)
	: m_gpu(gpu)
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

	serializer->write_string(shaders_source);
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

	serializer->read_string(shaders_source);

	this->reload();
}

void titian::Material::reload()
{
	if (shaders_source.empty()) {
		shaders = {};
		return;
	}

	const std::string processed_source = process_shader_source();
	shaders = m_gpu->create_render_shaders(processed_source);
}

bool titian::Material::is_transparent() const
{
	return alpha_blend < 1.0f;
}

std::string titian::Material::process_shader_source() const
{
	std::stringstream shader_sources[2] = {};
	int shader_process_type = -1;
	for (const auto& line : kl::split_string(shaders_source, '\n')) {
		if (line.find("#define _CUSTOM_VERTEX_SHADER") != -1) {
			shader_process_type = 0;
		}
		if (line.find("#define _CUSTOM_PIXEL_SHADER") != -1) {
			shader_process_type = 1;
		}
		if (shader_process_type >= 0) {
			shader_sources[shader_process_type] << line << '\n';
		}
	}

	std::stringstream full_source{};
	for (const auto& line : kl::split_string(kl::read_file_string("builtin/shaders/scene_pass.hlsl"), '\n')) {
		if (line.find("#define _CUSTOM_VERTEX_SHADER_PLACEHOLDER") != -1) {
			full_source << shader_sources[0].str() << '\n';
		}
		else if (line.find("#define _CUSTOM_PIXEL_SHADER_PLACEHOLDER") != -1) {
			full_source << shader_sources[1].str() << '\n';
		}
		else {
			full_source << line << '\n';
		}
	}
	return full_source.str();
}
