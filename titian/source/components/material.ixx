export module material;

export import unique;

export namespace titian {
	class Material : public Unique
	{
	public:
		float texture_blend = 0.0f;
		float reflection_factor = 0.0f;
		float refraction_factor = 0.0f;
		float refraction_index = 1.0f;

		kl::Float4 color = kl::colors::WHITE;

		std::string color_map = "/";
		std::string normal_map = "/";
		std::string roughness_map = "/";

		Material()
		{}

		~Material() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Unique::serialize(serializer);

			serializer->write_object(texture_blend);
			serializer->write_object(reflection_factor);
			serializer->write_object(refraction_factor);
			serializer->write_object(refraction_index);
			serializer->write_object(color);

			serializer->write_string(color_map);
			serializer->write_string(normal_map);
			serializer->write_string(roughness_map);
		}

		void deserialize(const Serializer* serializer) override
		{
			Unique::deserialize(serializer);

			serializer->read_object(texture_blend);
			serializer->read_object(reflection_factor);
			serializer->read_object(refraction_factor);
			serializer->read_object(refraction_index);
			serializer->read_object(color);

			serializer->read_string(color_map);
			serializer->read_string(normal_map);
			serializer->read_string(roughness_map);
		}
	};
}
