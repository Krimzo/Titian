#pragma once

#include "serialization/serialization.h"


namespace titian {
	class Material : public Serializable
	{
	public:
		Float4 color = kl::colors::WHITE;
		float texture_blend = 0.0f;

		float reflection_factor = 0.0f;
		float refraction_factor = 0.0f;
		float refraction_index = 1.0f;

		Float4x4 custom_data = {};

		String color_map_name = "/";
		String normal_map_name = "/";
		String roughness_map_name = "/";

		String shader_name = "/";

		Material();

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		bool is_transparent() const;
	};
}
