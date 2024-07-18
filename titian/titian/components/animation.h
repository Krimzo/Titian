#pragma once

#include "serialization/serializable.h"


namespace titian {
	inline constexpr int MAX_BONE_COUNT = 100;
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
	class Animation : public Serializable
	{
	public:
		AnimationType type = AnimationType::SEQUENTIAL;
		std::vector<std::string> meshes;
		float fps = 30.0f;

		Animation(Scene* scene);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		int get_index(float index) const;
		Mesh* get_mesh(float time) const;

		void update_matrices(float current_time);
		void load_matrices(kl::Float4x4* out_data) const;

	private:
		Scene* m_scene = nullptr;
	};
}
