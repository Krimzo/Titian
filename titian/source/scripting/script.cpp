#include "titian.h"


void titian::Script::serialize(Serializer* serializer, const void* helper_data) const
{
	serializer->write_string("script_type", typeid(self).name()); // must be read from outside
}

void titian::Script::deserialize(const Serializer* serializer, const void* helper_data)
{}
