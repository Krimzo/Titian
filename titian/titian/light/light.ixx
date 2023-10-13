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

		void serialize(Serializer* serializer) const override
		{
			Entity::serialize(serializer);
		}

		void deserialize(const Serializer* serializer) override
		{
			Entity::deserialize(serializer);
		}

		virtual kl::Float3 light_at_point(const kl::Float3& point) const = 0;
	};
}
