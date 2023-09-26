export module material;

export import texture;

export namespace titian {
	class Material : public Unique
	{
	public:
		float texture_blend = 0.0f;
		float reflection_factor = 0.0f;
		float refraction_factor = 0.0f;
		float refraction_index = 1.0f;

		kl::Float4 color = kl::colors::WHITE;

		kl::Object<Texture> color_map = nullptr;
		kl::Object<Texture> normal_map = nullptr;
		kl::Object<Texture> roughness_map = nullptr;

		Material()
		{}

		~Material() override
		{}

		void serialize(kl::File* file) const override
		{
			Unique::serialize(file);
			file->write(texture_blend);
			file->write(reflection_factor);
			file->write(refraction_factor);
			file->write(refraction_index);
			file->write(color);
		}

		void deserialize(const kl::File* file) override
		{
			Unique::deserialize(file);
			file->read(texture_blend);
			file->read(reflection_factor);
			file->read(refraction_factor);
			file->read(refraction_index);
			file->read(color);
		}
	};
}
