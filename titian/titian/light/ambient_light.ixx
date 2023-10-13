export module ambient_light;

export import light;

export namespace titian {
	class AmbientLight : public Light
	{
	public:
		kl::Float3 color{ 0.1f };

		AmbientLight(physx::PxPhysics* physics, const bool dynamic)
			: Light(physics, dynamic)
		{}

		~AmbientLight() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Light::serialize(serializer);

			serializer->write_object(color);
		}

		void deserialize(const Serializer* serializer) override
		{
			Light::deserialize(serializer);

			serializer->read_object(color);
		}

		kl::Float3 light_at_point(const kl::Float3& point) const override
		{
			return color;
		}
	};
}
