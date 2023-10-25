export module material;

export import serializable;

export namespace titian {
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

		Material()
		{}

		~Material() override
		{}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			serializer->write_object<float>(texture_blend);
			serializer->write_object<float>(reflection_factor);
			serializer->write_object<float>(refraction_factor);
			serializer->write_object<float>(refraction_index);

			serializer->write_object<kl::Float4>(color);

			serializer->write_string(color_map_name);
			serializer->write_string(normal_map_name);
			serializer->write_string(roughness_map_name);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			serializer->read_object<float>(texture_blend);
			serializer->read_object<float>(reflection_factor);
			serializer->read_object<float>(refraction_factor);
			serializer->read_object<float>(refraction_index);

			serializer->read_object<kl::Float4>(color);

			serializer->read_string(color_map_name);
			serializer->read_string(normal_map_name);
			serializer->read_string(roughness_map_name);
		}
	};
}
