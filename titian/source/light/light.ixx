export module light;

export import entity;

export namespace titian {
	class Light : public Entity
	{
	public:
		Light(physx::PxPhysics* physics, const bool dynamic)
			: Entity(physics, dynamic)
		{}

		~Light() override
		{}

		void serialize(kl::File* file) const override
		{
			Entity::serialize(file);
		}

		void deserialize(const kl::File* file) override
		{
			Entity::deserialize(file);
		}

		virtual kl::Float3 light_at_point(const kl::Float3& point) const = 0;
	};
}
