#include "main.h"


titian::AmbientLight::AmbientLight(physx::PxPhysics* physics, const bool dynamic)
	: Light(EntityType::AMBIENT_LIGHT, physics, dynamic)
{}

void titian::AmbientLight::serialize(Serializer* serializer, const void* helper_data) const
{
	Light::serialize(serializer, helper_data);

	serializer->write_object<float>(intensity);
	serializer->write_object<kl::Float3>(color);
}

void titian::AmbientLight::deserialize(const Serializer* serializer, const void* helper_data)
{
	Light::deserialize(serializer, helper_data);

	serializer->read_object<float>(intensity);
	serializer->read_object<kl::Float3>(color);
}

kl::Float3 titian::AmbientLight::light_at_point(const kl::Float3& point) const
{
	return color * intensity;
}
