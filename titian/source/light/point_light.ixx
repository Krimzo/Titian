export module point_light;

export import light;

export namespace titian {
	class PointLight : public Light
	{
	public:
		kl::Float3 position = {};
		kl::Float3 color{ 1.0f };

		PointLight()
		{}

		~PointLight() override
		{}

		void serialize(kl::File* file) const override
		{
			Light::serialize(file);
			file->write(position);
			file->write(color);
		}

		void deserialize(const kl::File* file) override
		{
			Light::deserialize(file);
			file->read(position);
			file->read(color);
		}

		kl::Float3 light_at_point(const kl::Float3& point) const override
		{
			return color; // Change later
		}
	};
}
