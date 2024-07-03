#include "main.h"


titian::Shader::Shader(ShaderType type, kl::GPU* gpu)
	: type(type), m_gpu(gpu)
{
	switch (type)
	{
		case ShaderType::MATERIAL: data_buffer = get_default_material_shader(); break;
		case ShaderType::CAMERA: data_buffer = get_default_camera_shader(); break;
	}
}

void titian::Shader::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_object<ShaderType>(type);
	serializer->write_string(data_buffer);
}

void titian::Shader::deserialize(const Serializer* serializer, const void* helper_data)
{
	serializer->read_object<ShaderType>(type);
	serializer->read_string(data_buffer);
	this->reload();
}

void titian::Shader::reload()
{
	if (type == ShaderType::MATERIAL) {
		reload_for_material();
	}
	else if (type == ShaderType::CAMERA) {
		reload_for_camera();
	}
}

void titian::Shader::reload_for_material()
{
	if (data_buffer.empty()) {
		graphics_buffer = {};
		return;
	}

	std::stringstream shader_sources[2] = {};
	int shader_process_type = -1;
	for (const auto& line : kl::split_string(data_buffer, '\n')) {
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
	graphics_buffer = m_gpu->create_render_shaders(full_source.str());
}

void titian::Shader::reload_for_camera()
{
	if (data_buffer.empty()) {
		graphics_buffer = {};
		return;
	}
	graphics_buffer = m_gpu->create_render_shaders(data_buffer);
}
