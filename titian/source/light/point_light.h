#pragma once

#include "scene/entity.h"


namespace titian {
	struct PointLight : Entity
	{
		Float3 color{ 1.0f };

		PointLight();

		void serialize(Serializer* serializer) const override;
		void deserialize(const Serializer* serializer) override;
	};
}
