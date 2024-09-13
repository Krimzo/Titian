#pragma once

#include "light/light.h"


namespace titian {
	struct PointLight : Light
	{
		Float3 color{ 1.0f };

		PointLight(px::PxPhysics* physics);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		Float3 light_at_point(const Float3& point) const override;
	};
}
