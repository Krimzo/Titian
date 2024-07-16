#pragma once

#include "serialization/serializable.h"


namespace titian {
	class Mesh;
	class Scene;
}

namespace titian {
	class Animation : public Serializable
	{
	public:
		std::vector<std::string> meshes = {};
		float fps = 30.0f;

		Animation(Scene* scene);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		int get_index(float index) const;
		Mesh* get_mesh(float time) const;

	private:
		Scene* m_scene = nullptr;
	};
}
