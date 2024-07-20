#include "main.h"


titian::PointLight::PointLight(px::PxPhysics* physics, const bool dynamic)
	: Light(EntityType::POINT_LIGHT, physics, dynamic)
{}

void titian::PointLight::serialize(Serializer* serializer, const void* helper_data) const
{
	Light::serialize(serializer, helper_data);

	serializer->write_object<Float3>(color);
}

void titian::PointLight::deserialize(const Serializer* serializer, const void* helper_data)
{
	Light::deserialize(serializer, helper_data);

	serializer->read_object<Float3>(color);
}

titian::Float3 titian::PointLight::light_at_point(const Float3& point) const
{
	return color; // Change later
}
