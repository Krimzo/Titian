#include "titian.h"


titian::Script::Script(const ScriptType type)
	: type(type)
{}

void titian::Script::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_object<ScriptType>(type);
}

void titian::Script::deserialize(const Serializer* serializer, const void* helper_data)
{}
