#include "titian.h"


titian::Script::Script(const ScriptType type)
	: script_type(type)
{}

void titian::Script::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_int("script_type", (int32_t) script_type);
}

void titian::Script::deserialize(const Serializer* serializer, const void* helper_data)
{}
