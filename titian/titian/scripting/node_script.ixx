export module node_script;

export import script;
export import node_script_types;

export namespace titian {
	class NodeScript : public Script
	{
	public:
		std::vector<kl::Object<FlowNode>> nodes = {};
		std::vector<kl::Object<FlowLink>> links = {};

		NodeScript(void* scene)
			: Script(Type::NODE, scene)
		{
			// Start
			kl::Object start_node = new FlowNode();
			start_node->title = "on start";
			start_node->set_position({0.f, 100.f});
			nodes.push_back(start_node);

			kl::Object start_node_output = new FlowNodePin();
			start_node_output->type = FlowNodePin::Type::FLOW;
			start_node_output->kind = FlowNodePin::Kind::OUTPUT;
			start_node->pins.push_back(start_node_output);
			
			// Update
			kl::Object update_node = new FlowNode();
			update_node->title = "on update";
			update_node->set_position({ 0.f, -100.f });
			nodes.push_back(update_node);

			kl::Object update_node_output = new FlowNodePin();
			update_node_output->type = FlowNodePin::Type::FLOW;
			update_node_output->kind = FlowNodePin::Kind::OUTPUT;
			update_node->pins.push_back(update_node_output);
		}

		~NodeScript() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Script::serialize(serializer);
		}

		void deserialize(const Serializer* serializer) override
		{
			Script::deserialize(serializer);
		}

		bool is_valid() const override
		{
			return false;
		}

		void reload() override
		{}

		void call_start() override
		{}

		void call_update() override
		{}
	};
}
