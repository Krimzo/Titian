export module visual_script;

export import script;
export import visual_script_types;

export namespace titian {
	class VisualScript : public Script
	{
	public:
		VisualNode start_node = {};
		VisualNode update_node = {};

		std::vector<VisualNode> nodes = {};
		std::vector<VisualLink> links = {};

		VisualScript(void* scene)
			: Script(Type::VISUAL, scene)
		{
			start_node.title = "Start";
			update_node.title = "Update";

			VisualPin start_out_pin = {};
			start_out_pin.name = "on start";
			start_out_pin.type = VisualPin::Type::FLOW;
			start_out_pin.kind = VisualPin::Kind::OUTPUT;
			start_node.pins.push_back(start_out_pin);

			VisualPin update_out_pin = {};
			update_out_pin.name = "on update";
			update_out_pin.type = VisualPin::Type::FLOW;
			update_out_pin.kind = VisualPin::Kind::OUTPUT;
			update_node.pins.push_back(update_out_pin);
		}

		~VisualScript() override
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
			return true;
		}

		void reload() override
		{}

		void call_start() override
		{}

		void call_update() override
		{}
	};
}
