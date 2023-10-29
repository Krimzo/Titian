export module point_light;

export import light;

export namespace titian {
	class PointLight : public Light
	{
	public:
		kl::Float3 color{ 1.0f };

		PointLight(physx::PxPhysics* physics, const bool dynamic)
			: Light(Type::POINT_LIGHT, physics, dynamic)
		{}

		~PointLight() override
		{}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			Light::serialize(serializer, helper_data);

			serializer->write_object<kl::Float3>(color);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			Light::deserialize(serializer, helper_data);

			serializer->read_object<kl::Float3>(color);
		}

		kl::Float3 light_at_point(const kl::Float3& point) const override
		{
			return color; // Change later
		}
	};
}
