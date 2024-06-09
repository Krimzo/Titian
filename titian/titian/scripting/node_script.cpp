#include "main.h"


titian::NodeScript::NodeScript()
	: Script(ScriptType::NODE)
{
	start_node.title = "on_start";
	start_node.flow_output = Pin("flow out", PinType::FLOW, nullptr);

	update_node.title = "on_update";
	update_node.flow_output = Pin("flow out", PinType::FLOW, nullptr);

	collision_node.title = "on_collision";
	collision_node.flow_output = Pin("flow out", PinType::FLOW, nullptr);
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

void titian::NodeScript::call_start(Scene* scene)
{
	start_node.call();
}

void titian::NodeScript::call_update(Scene* scene)
{
	update_node.call();
}

void titian::NodeScript::call_collision(Scene* scene, Entity* first, Entity* second)
{
	collision_node.call();
}
