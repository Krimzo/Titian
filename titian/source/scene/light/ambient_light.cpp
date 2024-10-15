#include "titian.h"


titian::AmbientLight::AmbientLight()
{}

void titian::AmbientLight::serialize(Serializer& serializer) const
{
	Entity::serialize(serializer);
	serializer.write_float_array("color", &color.x, 3);
}

void titian::AmbientLight::deserialize(const Serializer& serializer)
{
	Entity::deserialize(serializer);
	serializer.read_float_array("color", &color.x, 3);
}
