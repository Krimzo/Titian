#include "titian.h"


titian::Animation::Animation(kl::GPU* gpu, Scene* scene)
	: m_gpu(gpu), m_scene(scene)
{
	dx::BufferDescriptor descriptor{};
	descriptor.Usage = D3D11_USAGE_DYNAMIC;
	descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	descriptor.StructureByteStride = sizeof(Float4x4);
	descriptor.ByteWidth = descriptor.StructureByteStride * MAX_BONE_COUNT;
	m_matrices_buffer = gpu->create_buffer(&descriptor, nullptr);
	kl::assert(m_matrices_buffer, "Failed to create animation matrices buffer");

	m_matrices_view = gpu->create_shader_view(m_matrices_buffer, nullptr);
	kl::assert(m_matrices_view, "Failed to create animation matrices shader view");
}

void titian::Animation::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_object<AnimationType>(type);
	serializer->write_object<float>(ticks_per_second);
	serializer->write_object<float>(duration_in_ticks);

	serializer->write_object<uint64_t>(meshes.size());
	for (auto& mesh : meshes) {
		serializer->write_string(mesh);
	}

	serializer->write_object<uint64_t>(channels.size());
	for (auto& channel : channels) {
		serializer->write_object<uint64_t>(channel.scalings.size());
		for (auto& scaling : channel.scalings) {
			serializer->write_object(scaling.first);
			serializer->write_object(scaling.second);
		}

		serializer->write_object<uint64_t>(channel.rotations.size());
		for (auto& rotation : channel.rotations) {
			serializer->write_object(rotation.first);
			serializer->write_object(rotation.second);
		}

		serializer->write_object<uint64_t>(channel.positions.size());
		for (auto& position : channel.positions) {
			serializer->write_object(position.first);
			serializer->write_object(position.second);
		}
	}

	Function<void(const AnimationNode*)> rec_helper;
	rec_helper = [&](const AnimationNode* node)
	{
		serializer->write_object<int>(node->channel_index);
		serializer->write_object<uint64_t>(node->children.size());
		for (auto& child : node->children) {
			rec_helper(&child);
		}
	};

	serializer->write_object<bool>((bool) animation_root);
	if (animation_root) {
		rec_helper(&animation_root);
	}
}

void titian::Animation::deserialize(const Serializer* serializer, const void* helper_data)
{
	serializer->read_object<AnimationType>(type);
	serializer->read_object<float>(ticks_per_second);
	serializer->read_object<float>(duration_in_ticks);

	meshes.resize(serializer->read_object<uint64_t>());
	for (auto& mesh : meshes) {
		serializer->read_string(mesh);
	}

	channels.resize(serializer->read_object<uint64_t>());
	for (auto& channel : channels) {
		channel.scalings.resize(serializer->read_object<uint64_t>());
		for (auto& scaling : channel.scalings) {
			serializer->read_object(scaling.first);
			serializer->read_object(scaling.second);
		}

		channel.rotations.resize(serializer->read_object<uint64_t>());
		for (auto& rotation : channel.rotations) {
			serializer->read_object(rotation.first);
			serializer->read_object(rotation.second);
		}

		channel.positions.resize(serializer->read_object<uint64_t>());
		for (auto& position : channel.positions) {
			serializer->read_object(position.first);
			serializer->read_object(position.second);
		}
	}

	Function<void(AnimationNode*)> rec_helper;
	rec_helper = [&](AnimationNode* node)
	{
		serializer->read_object<int>(node->channel_index);
		node->children.resize(serializer->read_object<uint64_t>());
		for (auto& child : node->children) {
			child = new AnimationNode();
			rec_helper(&child);
		}
	};

	if (serializer->read_object<bool>()) {
		animation_root = new AnimationNode();
		rec_helper(&animation_root);
	}
}

int titian::Animation::get_index(const float time) const
{
	if (meshes.empty()) {
		return -1;
	}
	return int(time * ticks_per_second) % (int) meshes.size();
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

void titian::Animation::update(const float current_time)
{
	if (type != AnimationType::SKELETAL || meshes.empty() || !this->animation_root) {
		return;
	}

	Mesh* mesh = m_scene->helper_get_mesh(meshes[0]);
	if (!mesh || !mesh->skeleton_root) {
		return;
	}

	const float tps = (ticks_per_second > 0) ? ticks_per_second : 30.0f;
	const float time_in_ticks = current_time * tps;
	const float animation_time_in_ticks = fmod(time_in_ticks, duration_in_ticks);

	m_global_inverse_transform = kl::inverse(mesh->skeleton_root->transformation);

	Float4x4 identity;
	update_node_hierarchy(animation_time_in_ticks, mesh, &mesh->skeleton_root, &this->animation_root, identity);
	upload_matrices();
}

void titian::Animation::bind_matrices(const int slot) const
{
	m_gpu->bind_shader_view_for_vertex_shader(m_matrices_view, slot);
}

void titian::Animation::upload_matrices()
{
	if (m_final_matrices.empty()) {
		return;
	}
	const size_t count = std::min(m_final_matrices.size(), (size_t) MAX_BONE_COUNT);
	m_gpu->write_to_buffer(m_matrices_buffer, m_final_matrices.data(), count * sizeof(Float4x4));
}

void titian::Animation::update_node_hierarchy(const float time_ticks, const Mesh* mesh, const SkeletonNode* skel_node, const AnimationNode* anim_node, const Float4x4& parent_transform)
{
	if (skel_node->children.size() != anim_node->children.size()) {
		Logger::log("Invalid animation and skeleton hierarchy");
		return;
	}

	Float4x4 node_transform = skel_node->transformation;
	if (anim_node->channel_index >= 0) {
		const aiVector3f translation = interpolate_translation(time_ticks, anim_node->channel_index);
		const aiQuaternion rotation = interpolate_rotation(time_ticks, anim_node->channel_index);
		const aiVector3f scaling = interpolate_scaling(time_ticks, anim_node->channel_index);
		node_transform = make_animation_matrix(translation, rotation, scaling);
	}
	Float4x4 global_transformation = parent_transform * node_transform;

	if (skel_node->bone_index >= 0) {
		if (skel_node->bone_index >= m_final_matrices.size()) {
			m_final_matrices.resize((size_t) skel_node->bone_index + 1);
		}
		m_final_matrices[skel_node->bone_index] = m_global_inverse_transform * global_transformation * mesh->bone_matrices[skel_node->bone_index];
	}

	for (size_t i = 0; i < skel_node->children.size(); i++) {
		update_node_hierarchy(time_ticks, mesh, &skel_node->children[i], &anim_node->children[i], global_transformation);
	}
}

aiVector3f titian::Animation::interpolate_translation(const float time_ticks, const int channel_index) const
{
	auto& keys = channels[channel_index].positions;
	if (keys.empty()) {
		return {};
	}
	if (keys.size() == 1) {
		return keys[0].second;
	}

	size_t key_index = 0;
	for (size_t i = 0; i < keys.size() - 1; i++) {
		if (time_ticks < keys[i + 1].first) {
			key_index = i;
			break;
		}
	}
	const size_t next_key_index = key_index + 1;

	const float t = (time_ticks - keys[key_index].first) / (keys[next_key_index].first - keys[key_index].first);
	return keys[key_index].second * (1.0f - t) + keys[next_key_index].second * t;
}

aiQuaternion titian::Animation::interpolate_rotation(const float time_ticks, const int channel_index) const
{
	auto& keys = channels[channel_index].rotations;
	if (keys.empty()) {
		return {};
	}
	if (keys.size() == 1) {
		return keys[0].second;
	}

	size_t key_index = 0;
	for (size_t i = 0; i < keys.size() - 1; i++) {
		if (time_ticks < keys[i + 1].first) {
			key_index = i;
			break;
		}
	}
	const size_t next_key_index = key_index + 1;

	const float t = (time_ticks - keys[key_index].first) / (keys[next_key_index].first - keys[key_index].first);
	aiQuaternion result;
	aiQuaternion::Interpolate(result, keys[key_index].second, keys[next_key_index].second, t);
	return result.Normalize();
}

aiVector3f titian::Animation::interpolate_scaling(const float time_ticks, const int channel_index) const
{
	auto& keys = channels[channel_index].scalings;
	if (keys.empty()) {
		return { 1.0f, 1.0f, 1.0f };
	}
	if (keys.size() == 1) {
		return keys[0].second;
	}

	size_t key_index = 0;
	for (size_t i = 0; i < keys.size() - 1; i++) {
		if (time_ticks < keys[i + 1].first) {
			key_index = i;
			break;
		}
	}
	size_t next_key_index = key_index + 1;

	const float t = (time_ticks - keys[key_index].first) / (keys[next_key_index].first - keys[key_index].first);
	return keys[key_index].second * (1.0f - t) + keys[next_key_index].second * t;
}

titian::Float4x4 titian::Animation::make_animation_matrix(const aiVector3f& translation, const aiQuaternion& rotation, const aiVector3f& scaling) const
{
	const aiMatrix4x4 matrix{ scaling, rotation, translation };

	Float4x4 result;
	for (int i = 0; i < 4; i++) {
		memcpy(&result(0, i), matrix[i], sizeof(float) * 4);
	}
	return result;
}
