#include "titian.h"


titian::Light::Light(const EntityType type, px::PxPhysics* physics, const bool dynamic)
	: Entity(type, physics, dynamic)
{}
