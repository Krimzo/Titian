#include "main.h"


titian::Light::Light(const EntityType type, physx::PxPhysics* physics, const bool dynamic)
	: Entity(type, physics, dynamic)
{}
