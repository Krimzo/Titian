#include "main.h"


titian::NodeScript::NodeScript()
	: Script(Type::NODE)
{
	start_node.title = "on_start";
	start_node.flow_output = Pin("flow out", PinType::FLOW, nullptr);

	update_node.title = "on_update";
	update_node.flow_output = Pin("flow out", PinType::FLOW, nullptr);
}

void titian::NodeScript::serialize(Serializer* serializer, const void* helper_data) const
{
	Script::serialize(serializer, helper_data);
}

void titian::NodeScript::deserialize(const Serializer* serializer, const void* helper_data)
{
	Script::deserialize(serializer, helper_data);

	this->reload();
}

bool titian::NodeScript::is_valid() const
{
	return true;
}

void titian::NodeScript::reload()
{}

void titian::NodeScript::call_start()
{
	start_node.call();
}

void titian::NodeScript::call_update()
{
	update_node.call();
}
