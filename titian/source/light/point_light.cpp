#include "titian.h"


titian::PointLight::PointLight(px::PxPhysics* physics, const bool dynamic)
	: Light(EntityType::POINT_LIGHT, physics, dynamic)
{}

void titian::PointLight::serialize(Serializer* serializer, const void* helper_data) const
{
	Light::serialize(serializer, helper_data);

	serializer->write_float_array("color", color, 3);
}

void titian::PointLight::deserialize(const Serializer* serializer, const void* helper_data)
{
	Light::deserialize(serializer, helper_data);

	serializer->read_float_array("color", color, 3);
}

titian::Float3 titian::PointLight::light_at_point(const Float3& point) const
{
	return color; // Change later
}
