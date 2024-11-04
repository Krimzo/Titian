#include "titian.h"


titian::Animation::Animation()
{
	kl::GPU& gpu = AppLayer::get().gpu;

	dx::BufferDescriptor descriptor{};
	descriptor.Usage = D3D11_USAGE_DYNAMIC;
	descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	descriptor.StructureByteStride = sizeof(Float4x4);
	descriptor.ByteWidth = descriptor.StructureByteStride * MAX_BONE_COUNT;
	m_matrices_buffer = gpu.create_buffer(&descriptor, nullptr);
	kl::assert(m_matrices_buffer, "Failed to create animation matrices buffer");

	m_matrices_view = gpu.create_shader_view(m_matrices_buffer, nullptr);
	kl::assert(m_matrices_view, "Failed to create animation matrices shader view");
}

void titian::Animation::serialize(Serializer& serializer) const
{
	serializer.write_int("animation_type", animation_type);
	serializer.write_float("ticks_per_second", ticks_per_second);
	serializer.write_float("duration_in_ticks", duration_in_ticks);

	serializer.write_int("meshes_size", (int32_t) meshes.size());
	int counter = 0;
	for (auto& mesh : meshes) {
		serializer.write_string(kl::format("__mesh_", counter), mesh);
		counter += 1;
	}

	serializer.write_int("channels_size", (int32_t) channels.size());
	counter = 0;
	for (auto& channel : channels) {
		serializer.push_object(kl::format("__channel_", counter));
		serializer.write_int("scalings_size", (int32_t) channel.scalings.size());
		int temp_counter = 0;
		for (auto& scaling : channel.scalings) {
			serializer.push_object(kl::format("__scaling_", temp_counter));
			serializer.write_float("scaling_first", scaling.first);
			serializer.write_float_array("scaling_second", &scaling.second.x, 3);
			serializer.pop_object();
			temp_counter += 1;
		}

		serializer.write_int("rotations_size", (int32_t) channel.rotations.size());
		temp_counter = 0;
		for (auto& rotation : channel.rotations) {
			serializer.push_object(kl::format("__rotation_", temp_counter));
			serializer.write_float("rotation_first", rotation.first);
			serializer.write_float_array("rotation_second", &rotation.second.w, 4);
			serializer.pop_object();
			temp_counter += 1;
		}

		serializer.write_int("positions_size", (int32_t) channel.positions.size());
		temp_counter = 0;
		for (auto& position : channel.positions) {
			serializer.push_object(kl::format("__position_", temp_counter));
			serializer.write_float("position_first", position.first);
			serializer.write_float_array("position_second", &position.second.x, 3);
			serializer.pop_object();
			temp_counter += 1;
		}
		serializer.pop_object();
		counter += 1;
	}

	Func<void(const AnimationNode&)> rec_helper;
	counter = 0;
	rec_helper = [&](const AnimationNode& node)
	{
		serializer.push_object(kl::format("__node_", counter++));
		serializer.write_int("channel_index", node.channel_index);
		serializer.write_int("children_size", (int32_t) node.children.size());
		for (auto& child : node.children) {
			rec_helper(*child);
		}
		serializer.pop_object();
	};

	serializer.write_bool("has_data", (bool) animation_root);
	if (animation_root) {
		rec_helper(*animation_root);
	}
}

void titian::Animation::deserialize(const Serializer& serializer)
{
	serializer.read_int("animation_type", animation_type);
	serializer.read_float("ticks_per_second", ticks_per_second);
	serializer.read_float("duration_in_ticks", duration_in_ticks);

	int32_t meshes_size = 0;
	serializer.read_int("meshes_size", meshes_size);
	meshes.resize(meshes_size);
	int counter = 0;
	for (auto& mesh : meshes) {
		serializer.read_string(kl::format("__mesh_", counter), mesh);
		counter += 1;
	}

	int32_t channels_size = 0;
	serializer.read_int("channels_size", channels_size);
	channels.resize(channels_size);
	counter = 0;
	for (auto& channel : channels) {
		serializer.load_object(kl::format("__channel_", counter));
		int32_t scalings_size = 0;
		serializer.read_int("scalings_size", scalings_size);
		channel.scalings.resize(scalings_size);
		int temp_counter = 0;
		for (auto& scaling : channel.scalings) {
			serializer.load_object(kl::format("__scaling_", temp_counter));
			serializer.read_float("scaling_first", scaling.first);
			serializer.read_float_array("scaling_second", &scaling.second.x, 3);
			serializer.unload_object();
			temp_counter += 1;
		}

		int32_t rotations_size = 0;
		serializer.read_int("rotations_size", rotations_size);
		channel.rotations.resize(rotations_size);
		temp_counter = 0;
		for (auto& rotation : channel.rotations) {
			serializer.load_object(kl::format("__rotation_", temp_counter));
			serializer.read_float("rotation_first", rotation.first);
			serializer.read_float_array("rotation_second", &rotation.second.w, 4);
			serializer.unload_object();
			temp_counter += 1;
		}

		int32_t positions_size = 0;
		serializer.read_int("positions_size", positions_size);
		channel.positions.resize(positions_size);
		temp_counter = 0;
		for (auto& position : channel.positions) {
			serializer.load_object(kl::format("__position_", temp_counter));
			serializer.read_float("position_first", position.first);
			serializer.read_float_array("position_second", &position.second.x, 3);
			serializer.unload_object();
			temp_counter += 1;
		}
		serializer.unload_object();
		counter += 1;
	}

	Func<void(AnimationNode&)> rec_helper;
	counter = 0;
	rec_helper = [&](AnimationNode& node)
	{
		serializer.load_object(kl::format("__node_", counter++));
		serializer.read_int("channel_index", node.channel_index);
		int32_t children_size = 0;
		serializer.read_int("children_size", children_size);
		node.children.resize(children_size);
		for (auto& child : node.children) {
			child = new AnimationNode();
			rec_helper(*child);
		}
		serializer.unload_object();
	};

	bool has_data = false;
	serializer.read_bool("has_data", has_data);
	if (has_data) {
		animation_root = new AnimationNode();
		rec_helper(*animation_root);
	}
}

int titian::Animation::get_index(const float time) const
{
	if (meshes.empty()) {
		return -1;
	}
	return int(time * ticks_per_second) % (int) meshes.size();
}

titian::Mesh* titian::Animation::get_mesh(const Scene& scene, const float time) const
{
	if (meshes.empty()) {
		return nullptr;
	}
	if (animation_type == AnimationType::SKELETAL) {
		return scene.helper_get_mesh(meshes[0]);
	}
	const int index = get_index(time);
	return scene.helper_get_mesh(meshes[index]);
}

void titian::Animation::update(const Scene& scene, const float current_time)
{
	if (animation_type != AnimationType::SKELETAL || meshes.empty() || !animation_root) {
		return;
	}

	Mesh* mesh = scene.helper_get_mesh(meshes[0]);
	if (!mesh || !mesh->skeleton_root) {
		return;
	}

	const float tps = (ticks_per_second > 0) ? ticks_per_second : 30.0f;
	const float time_in_ticks = current_time * tps;
	const float animation_time_in_ticks = fmod(time_in_ticks, duration_in_ticks);
	m_global_inverse_transform = kl::inverse(mesh->skeleton_root->transformation);

	Float4x4 identity;
	update_node_hierarchy(animation_time_in_ticks, *mesh, *mesh->skeleton_root, *animation_root, identity);
	upload_matrices();
}

void titian::Animation::bind_matrices(const int slot) const
{
	kl::GPU& gpu = AppLayer::get().gpu;
	gpu.bind_shader_view_for_vertex_shader(m_matrices_view, slot);
}

titian::Ref<titian::Animation> titian::Animation::clone() const
{
	Ref animation = new Animation();
	animation->animation_type = animation_type;
	animation->ticks_per_second = ticks_per_second;
	animation->duration_in_ticks = duration_in_ticks;
	animation->meshes = meshes;
	animation->channels = channels;

	Func<void(const AnimationNode*, AnimationNode*)> rec_helper;
	rec_helper = [&](const AnimationNode* source, AnimationNode* target)
	{
		if (!source || !target)
			return;
		target->channel_index = source->channel_index;
		target->children.resize(source->children.size());
		for (int i = 0; i < source->children.size(); i++) {
			target->children[i] = new AnimationNode();
			rec_helper(&source->children[i], &target->children[i]);
		}
	};
	rec_helper(&animation_root, &animation->animation_root);

	return animation;
}

void titian::Animation::upload_matrices()
{
	if (m_final_matrices.empty()) {
		return;
	}
	kl::GPU& gpu = AppLayer::get().gpu;
	const size_t count = kl::min(m_final_matrices.size(), (size_t) MAX_BONE_COUNT);
	gpu.write_to_buffer(m_matrices_buffer, m_final_matrices.data(), count * sizeof(Float4x4));
}

void titian::Animation::update_node_hierarchy(const float time_ticks, const Mesh& mesh, const SkeletonNode& skel_node, const AnimationNode& anim_node, const Float4x4& parent_transform)
{
	if (skel_node.children.size() != anim_node.children.size()) {
		Logger::log("Invalid animation and skeleton hierarchy");
		return;
	}

	Float4x4 node_transform = skel_node.transformation;
	if (anim_node.channel_index >= 0) {
		const aiVector3f translation = interpolate_translation(time_ticks, anim_node.channel_index);
		const aiQuaternion rotation = interpolate_rotation(time_ticks, anim_node.channel_index);
		const aiVector3f scaling = interpolate_scaling(time_ticks, anim_node.channel_index);
		node_transform = make_animation_matrix(translation, rotation, scaling);
	}
	Float4x4 global_transformation = parent_transform * node_transform;

	if (skel_node.bone_index >= 0) {
		if (skel_node.bone_index >= m_final_matrices.size()) {
			m_final_matrices.resize((size_t) skel_node.bone_index + 1);
		}
		m_final_matrices[skel_node.bone_index] = m_global_inverse_transform * global_transformation * mesh.bone_matrices[skel_node.bone_index];
	}

	for (size_t i = 0; i < skel_node.children.size(); i++) {
		update_node_hierarchy(time_ticks, mesh, *skel_node.children[i], *anim_node.children[i], global_transformation);
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
		kl::copy<float>(&result(0, i), matrix[i], 4);
	}
	return result;
}
