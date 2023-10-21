export module node_script_types;

export import node_script_parts;

export namespace titian {
	std::map<std::string, std::function<kl::Object<FlowNode>()>> NODE_TYPES = {};
}

int dont_care = [&]
{
	using namespace titian;

	NODE_TYPES["add"] = []()
	{
		kl::Object node = new FlowNode();
		node->title = "add";
		return node;
	};

	NODE_TYPES["sub"] = []()
	{
		kl::Object node = new FlowNode();
		node->title = "sub";
		return node;
	};

	NODE_TYPES["mul"] = []()
	{
		kl::Object node = new FlowNode();
		node->title = "mul";
		return node;
	};

	NODE_TYPES["div"] = []()
	{
		kl::Object node = new FlowNode();
		node->title = "div";
		return node;
	};

	return 0;
}();
