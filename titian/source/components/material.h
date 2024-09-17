#pragma once

#include "serialization/serialization.h"


namespace titian {
	struct Material : Serializable
	{
		Float4 color = kl::colors::WHITE;
		float texture_blend = 0.0f;

		float reflection_factor = 0.0f;
		float refraction_factor = 0.0f;
		float refraction_index = 1.0f;

		Float4x4 custom_data;

		String color_texture_name = "/";
		String normal_texture_name = "/";
		String roughness_texture_name = "/";
		String shader_name = "/";

		Material();

		void serialize(Serializer* serializer) const override;
		void deserialize(const Serializer* serializer) override;

		bool is_transparent() const;
	};
}
