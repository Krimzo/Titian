#include "main.h"


titian::Light::Light(const Type type, physx::PxPhysics* physics, const bool dynamic)
	: Entity(type, physics, dynamic)
{}
