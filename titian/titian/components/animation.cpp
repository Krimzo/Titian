#include "main.h"


titian::Animation::Animation(Scene* scene)
	: m_scene(scene)
{}

void titian::Animation::serialize(Serializer* serializer, const void* helper_data) const
{
	const uint64_t size = meshes.size();
	serializer->write_object<uint64_t>(size);
	for (auto& mesh : meshes) {
		serializer->write_string(mesh);
	}
	serializer->write_object<float>(fps);
}

void titian::Animation::deserialize(const Serializer* serializer, const void* helper_data)
{
	const uint64_t size = serializer->read_object<uint64_t>();
	meshes.resize(size);
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
	const int index = get_index(time);
	if (index < 0) {
		return nullptr;
	}
	return m_scene->helper_get_mesh(meshes[index]);
}
