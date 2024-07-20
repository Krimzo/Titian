#pragma once

#include "serialization/serializable.h"


namespace titian {
	inline constexpr int MAX_BONE_COUNT = 256;
	inline constexpr int MAX_BONE_REFS = 4;
}

namespace titian {
	class Mesh;
	class Scene;
}

namespace titian {
	enum AnimationType : int
	{
		SEQUENTIAL = 0,
		SKELETAL = 1,
	};
}

namespace titian {
	struct AnimationChannel
	{
		Vector<Pair<float, aiVector3f>> scalings;
		Vector<Pair<float, aiQuaternion>> rotations;
		Vector<Pair<float, aiVector3f>> positions;
	};
}

namespace titian {
	struct AnimationNode
	{
		int channel_index = -1;
		Vector<Ref<AnimationNode>> children;
	};
}

namespace titian {
	class Animation : public Serializable
	{
	public:
		AnimationType type = AnimationType::SEQUENTIAL;
		float ticks_per_second = 30.0f;
		float duration_in_ticks = 0.0f;

		Vector<String> meshes;

		Vector<AnimationChannel> channels;
		Ref<AnimationNode> animation_root;

		Animation(kl::GPU* gpu, Scene* scene);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		int get_index(float index) const;
		Mesh* get_mesh(float time) const;

		void update(float current_time);
		void bind_matrices(int slot) const;

	private:
		kl::GPU* m_gpu = nullptr;
		Scene* m_scene = nullptr;

		kl::Float4x4 m_global_inverse_transform;
		Vector<kl::Float4x4> m_final_matrices;

		kl::dx::Buffer m_matrices_buffer;
		kl::dx::ShaderView m_matrices_view;

		void upload_matrices();
		void update_node_hierarchy(float time_ticks, const Mesh* mesh, const SkeletonNode* skel_node, const AnimationNode* anim_node, const kl::Float4x4& parent_transform);

		aiVector3f interpolate_translation(float time_ticks, int channel_index) const;
		aiQuaternion interpolate_rotation(float time_ticks, int channel_index) const;
		aiVector3f interpolate_scaling(float time_ticks, int channel_index) const;

		kl::Float4x4 make_animation_matrix(const aiVector3f& translation, const aiQuaternion& rotation, const aiVector3f& scaling) const;
	};
}
