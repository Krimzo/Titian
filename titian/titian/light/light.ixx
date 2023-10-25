export module light;

export import entity;

export namespace titian {
	class Light : public Entity
	{
	public:
		Light(const Type type, physx::PxPhysics* physics, const bool dynamic)
			: Entity(type, physics, dynamic)
		{}

		~Light() override
		{}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			Entity::serialize(serializer, helper_data);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			Entity::deserialize(serializer, helper_data);
		}

		virtual kl::Float3 light_at_point(const kl::Float3& point) const = 0;
	};
}
