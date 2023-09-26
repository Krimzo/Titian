export module directional_light;

export import light;

export namespace titian {
	class DirectionalLight : public Light
	{
		kl::Float3 m_direction = { 0.0f, -1.0f, 0.0f };

	public:
		kl::Float3 color{ 1.0f };

		DirectionalLight()
		{}

		~DirectionalLight() override
		{}

		void serialize(kl::File* file) const override
		{
			Light::serialize(file);
			file->write(m_direction);
			file->write(color);
		}

		void deserialize(const kl::File* file) override
		{
			Light::deserialize(file);
			file->read(m_direction);
			file->read(color);
		}

		kl::Float3 light_at_point(const kl::Float3& point) const override
		{
			return color; // Change later
		}
	};
}
