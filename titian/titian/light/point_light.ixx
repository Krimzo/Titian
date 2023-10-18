export module point_light;

export import light;

export namespace titian {
	class PointLight : public Light
	{
	public:
		kl::Float3 position = {};
		kl::Float3 color{ 1.0f };

		PointLight(physx::PxPhysics* physics, const bool dynamic)
			: Light(Type::POINT_LIGHT, physics, dynamic)
		{}

		~PointLight() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Light::serialize(serializer);

			serializer->write_object<kl::Float3>(position);
			serializer->write_object<kl::Float3>(color);
		}

		void deserialize(const Serializer* serializer) override
		{
			Light::deserialize(serializer);

			serializer->read_object<kl::Float3>(position);
			serializer->read_object<kl::Float3>(color);
		}

		kl::Float3 light_at_point(const kl::Float3& point) const override
		{
			return color; // Change later
		}
	};
}
