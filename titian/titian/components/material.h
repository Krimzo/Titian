#pragma once

#include "serialization/serializable.h"


namespace titian {
	class Material : public Serializable
	{
	public:
		kl::RenderShaders shaders{};

		float texture_blend = 0.0f;
		float reflection_factor = 0.0f;
		float refraction_factor = 0.0f;
		float refraction_index = 1.0f;

		kl::Float4 color = kl::colors::WHITE;

		std::string color_map_name = "/";
		std::string normal_map_name = "/";
		std::string roughness_map_name = "/";

		std::string shader_source{};
		std::string shader_source_file{};

		Material(kl::GPU* gpu);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		void reload();

	private:
		kl::GPU* m_gpu = nullptr;

		std::string process_shader_source() const;
	};
}
