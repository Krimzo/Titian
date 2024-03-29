#include "main.h"


titian::Shader::Shader(kl::GPU* gpu)
	: m_gpu(gpu)
{
	this->data_buffer = get_default_shader();
}

void titian::Shader::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_string(data_buffer);
}

void titian::Shader::deserialize(const Serializer* serializer, const void* helper_data)
{
	serializer->read_string(data_buffer);
	this->reload();
}

void titian::Shader::reload()
{
	if (data_buffer.empty()) {
		graphics_buffer = {};
		return;
	}

	const std::string processed_source = process_source();
	graphics_buffer = m_gpu->create_render_shaders(processed_source);
}

std::string titian::Shader::process_source() const
{
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
	return full_source.str();
}
