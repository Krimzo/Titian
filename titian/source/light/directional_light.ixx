export module directional_light;

export import light;

export namespace titian {
	class DirectionalLight : public Light
	{
		kl::Float3 m_direction = { 0.0f, -1.0f, 0.0f };

	public:
		kl::Float3 color{ 1.0f };

		DirectionalLight(physx::PxPhysics* physics, const bool dynamic)
			: Light(physics, dynamic)
		{}

		~DirectionalLight() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Light::serialize(serializer);

			serializer->write_object(m_direction);
			serializer->write_object(color);
		}

		void deserialize(const Serializer* serializer) override
		{
			Light::deserialize(serializer);

			serializer->read_object(m_direction);
			serializer->read_object(color);
		}

		kl::Float3 light_at_point(const kl::Float3& point) const override
		{
			return color; // Change later
		}
	};
}
