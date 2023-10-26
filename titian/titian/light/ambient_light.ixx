export module ambient_light;

export import light;

export namespace titian {
	class AmbientLight : public Light
	{
	public:
		kl::Float3 color = kl::colors::WHITE;
		float intensity = 0.1f;

		AmbientLight(physx::PxPhysics* physics, const bool dynamic)
			: Light(Type::AMBIENT_LIGHT, physics, dynamic)
		{}

		~AmbientLight() override
		{}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			Light::serialize(serializer, helper_data);

			serializer->write_object<float>(intensity);
			serializer->write_object<kl::Float3>(color);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			Light::deserialize(serializer, helper_data);

			serializer->read_object<float>(intensity);
			serializer->read_object<kl::Float3>(color);
		}

		kl::Float3 light_at_point(const kl::Float3& point) const override
		{
			return color * intensity;
		}
	};
}
