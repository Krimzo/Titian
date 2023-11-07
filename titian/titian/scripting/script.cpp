#include "main.h"


titian::Script::Script(const Type type)
	: type(type)
{}

void titian::Script::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_object<Type>(type);
}

void titian::Script::deserialize(const Serializer* serializer, const void* helper_data)
{}
