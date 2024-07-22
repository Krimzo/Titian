#include "titian.h"


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

	StringStream shader_sources[2] = {};
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

	StringStream full_source{};
	for (const auto& line : kl::split_string(kl::read_file_string("package/shaders/scene_pass.hlsl"), '\n')) {
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

	const Vector<dx::LayoutDescriptor> vertex_layout_descriptors = {
		{    "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{     "KL_Texture", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{      "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_BoneIndices", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_BoneWeights", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	graphics_buffer = m_gpu->create_render_shaders(full_source.str(), vertex_layout_descriptors);
}

void titian::Shader::reload_for_camera()
{
	if (data_buffer.empty()) {
		graphics_buffer = {};
		return;
	}
	graphics_buffer = m_gpu->create_render_shaders(data_buffer);
}
