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

		void serialize(kl::File* file) const override
		{
			Light::serialize(file);
			file->write(color);
		}

		void deserialize(const kl::File* file) override
		{
			Light::deserialize(file);
			file->read(color);
		}

		kl::Float3 light_at_point(const kl::Float3& point) const override
		{
			return color;
		}
	};
}
