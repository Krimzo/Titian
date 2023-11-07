#pragma once

#include "light/light.h"


namespace titian {
	class PointLight : public Light
	{
	public:
		kl::Float3 color{ 1.0f };

		PointLight(physx::PxPhysics* physics, bool dynamic);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		kl::Float3 light_at_point(const kl::Float3& point) const override;
	};
}
