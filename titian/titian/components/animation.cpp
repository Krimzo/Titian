#include "main.h"


titian::Animation::Animation(Scene* scene)
	: m_scene(scene)
{}

void titian::Animation::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_object<AnimationType>(type);

	serializer->write_object<uint64_t>(meshes.size());
	for (auto& mesh : meshes) {
		serializer->write_string(mesh);
	}

	serializer->write_object<float>(fps);
}

void titian::Animation::deserialize(const Serializer* serializer, const void* helper_data)
{
	serializer->read_object<AnimationType>(type);

	meshes.resize(serializer->read_object<uint64_t>());
	for (auto& mesh : meshes) {
		serializer->read_string(mesh);
	}

	serializer->read_object<float>(fps);
}

int titian::Animation::get_index(const float time) const
{
	if (meshes.empty()) {
		return -1;
	}
	return int(time * fps) % (int) meshes.size();
}

titian::Mesh* titian::Animation::get_mesh(const float time) const
{
	if (meshes.empty()) {
		return nullptr;
	}
	if (type == AnimationType::SKELETAL) {
		return m_scene->helper_get_mesh(meshes[0]);
	}
	const int index = get_index(time);
	return m_scene->helper_get_mesh(meshes[index]);
}

void titian::Animation::update_matrices(const float current_time)
{
}

void titian::Animation::load_matrices(kl::Float4x4* out_data) const
{
}
