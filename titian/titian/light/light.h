#pragma once

#include "scene/entity.h"


namespace titian {
	class Light : public Entity
	{
	public:
		Light(EntityType type, px::PxPhysics* physics, bool dynamic);

		virtual kl::Float3 light_at_point(const kl::Float3& point) const = 0;
	};
}
