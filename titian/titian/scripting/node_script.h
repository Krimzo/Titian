#pragma once

#include "scripting/script.h"
#include "scripting/nodes/scripting_nodes.h"
#include "scene/scene.h"


namespace titian {
	class NodeScript : public Script
	{
	public:
		Node start_node;
		Node update_node;
		std::vector<Node> nodes;

		NodeScript();

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		bool is_valid() const override;
		void reload() override;
		void call_start() override;
		void call_update() override;
	};
}
