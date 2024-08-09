#include "titian.h"


titian::AmbientLight::AmbientLight(px::PxPhysics* physics, const bool dynamic)
	: Light(EntityType::AMBIENT_LIGHT, physics, dynamic)
{}

void titian::AmbientLight::serialize(Serializer* serializer, const void* helper_data) const
{
	Light::serialize(serializer, helper_data);

	serializer->write_float("intensity", intensity);
	serializer->write_float_array("color", color, 3);
}

void titian::AmbientLight::deserialize(const Serializer* serializer, const void* helper_data)
{
	Light::deserialize(serializer, helper_data);

	serializer->read_float("intensity", intensity);
	serializer->read_float_array("color", color, 3);
}

titian::Float3 titian::AmbientLight::light_at_point(const Float3& point) const
{
	return color * intensity;
}
