#pragma once

#include "serialization/serializable.h"


namespace titian {
	class Material : public Serializable
	{
	public:
		float texture_blend = 0.0f;
		float reflection_factor = 0.0f;
		float refraction_factor = 0.0f;
		float refraction_index = 1.0f;

		kl::Float4 color = kl::colors::WHITE;

		std::string color_map_name = "/";
		std::string normal_map_name = "/";
		std::string roughness_map_name = "/";

		Material() = default;

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;
	};
}
