#include "titian.h"


titian::PointLight::PointLight()
{}

void titian::PointLight::serialize(Serializer* serializer) const
{
	Entity::serialize(serializer);

	serializer->write_float_array("color", &color.x, 3);
}

void titian::PointLight::deserialize(const Serializer* serializer)
{
	Entity::deserialize(serializer);

	serializer->read_float_array("color", &color.x, 3);
}
