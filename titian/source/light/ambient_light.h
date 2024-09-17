#pragma once

#include "scene/entity.h"


namespace titian {
	struct AmbientLight : Entity
	{
		Float3 color = kl::colors::WHITE;
		float intensity = 0.1f;

		AmbientLight();

		void serialize(Serializer* serializer) const override;
		void deserialize(const Serializer* serializer) override;
	};
}
