#include "titian.h"


namespace titian {
	template<typename T, typename... Args>
	static constexpr Pair<String, Function<std::shared_ptr<Node>()>> serial_generator_helper(const Args&... args)
	{
		return { typeid(T).name(), [=]() { return std::shared_ptr<T>(new T(args...)); } };
	}

	static const Map<String, Function<std::shared_ptr<Node>()>> serial_node_generators
	{
		serial_generator_helper<Node>("unknown", ne::NodeStyle::red()),

		serial_generator_helper<LiteralNode<bool>>(""),
		serial_generator_helper<LiteralNode<int32_t>>(""),
		serial_generator_helper<LiteralNode<float>>(""),
		serial_generator_helper<LiteralNode<String>>(""),

		serial_generator_helper<CastNode<bool, int32_t>>(""),
		serial_generator_helper<CastNode<bool, float>>(""),
		serial_generator_helper<CastNode<bool, String>>(""),

		serial_generator_helper<CastNode<int32_t, bool>>(""),
		serial_generator_helper<CastNode<int32_t, float>>(""),
		serial_generator_helper<CastNode<int32_t, String>>(""),

		serial_generator_helper<CastNode<float, bool>>(""),
		serial_generator_helper<CastNode<float, int32_t>>(""),
		serial_generator_helper<CastNode<float, String>>(""),

		serial_generator_helper<CastNode<String, bool>>(""),
		serial_generator_helper<CastNode<String, int32_t>>(""),
		serial_generator_helper<CastNode<String, float>>(""),

		serial_generator_helper<FlowNode>("", false, false),

		serial_generator_helper<IfNode>(),
		serial_generator_helper<WhileNode>(),
		serial_generator_helper<ForNode>(),
		serial_generator_helper<PrintNode>(),
	};
}

namespace titian {
	template<typename T, typename... Args>
	static constexpr Pair<String, Function<std::shared_ptr<Node>()>> ui_generator_helper(const char* name, const Args&... args)
	{
		return { name, [=]() { return std::shared_ptr<T>(new T(args...)); } };
	}

	static const std::map<String, Function<std::shared_ptr<Node>()>> ui_node_generators = {
		ui_generator_helper<LiteralNode<bool>>("Bool", "Bool"),
		ui_generator_helper<LiteralNode<int32_t>>("Int", "Int"),
		ui_generator_helper<LiteralNode<float>>("Float", "Float"),
		ui_generator_helper<LiteralNode<String>>("String", "String"),

		ui_generator_helper<CastNode<bool, int32_t>>("Bool -> Int", "Bool -> Int"),
		ui_generator_helper<CastNode<bool, float>>("Bool -> Float", "Bool -> Float"),
		ui_generator_helper<CastNode<bool, String>>("Bool -> String", "Bool -> String"),

		ui_generator_helper<CastNode<int32_t, bool>>("Int -> Bool", "Int -> Bool"),
		ui_generator_helper<CastNode<int32_t, float>>("Int -> Float", "Int -> Float"),
		ui_generator_helper<CastNode<int32_t, String>>("Int -> String", "Int -> String"),

		ui_generator_helper<CastNode<float, bool>>("Float -> Bool", "Float -> Bool"),
		ui_generator_helper<CastNode<float, int32_t>>("Float -> Int", "Float -> Int"),
		ui_generator_helper<CastNode<float, String>>("Float -> String", "Float -> String"),

		ui_generator_helper<CastNode<String, bool>>("String -> Bool",  "String -> Bool"),
		ui_generator_helper<CastNode<String, int32_t>>("String -> Int", "String -> Int"),
		ui_generator_helper<CastNode<String, float>>("String -> Float",  "String -> Float"),

		ui_generator_helper<IfNode>("If"),
		ui_generator_helper<WhileNode>("While"),
		ui_generator_helper<ForNode>("For"),

		ui_generator_helper<PrintNode>("Print"),
	};
}

titian::NodeScript::NodeScript()
{
	kl::Timer* timer = &Layers::get<AppLayer>()->timer;

	auto temp_start_node = std::make_shared<FlowNode>("On Start", false, true);
	auto temp_update_node = std::make_shared<FlowNode>("On Update", false, true);
	auto temp_collision_node = std::make_shared<FlowNode>("On Collision", false, true);
	auto temp_ui_node = std::make_shared<FlowNode>("On UI", false, true);

	temp_start_node->setUID(1);
	temp_update_node->setUID(2);
	temp_collision_node->setUID(3);
	temp_ui_node->setUID(4);

	on_start_node = m_editor.insertNode({ 0.0f, -250.0f }, temp_start_node).get();
	on_update_node = m_editor.insertNode({ 0.0f, -83.33f }, temp_update_node).get();
	on_collision_node = m_editor.insertNode({ 0.0f, 83.33f }, temp_collision_node).get();
	on_ui_node = m_editor.insertNode({ 0.0f, 250.0f }, temp_ui_node).get();

	on_update_node->addOUT<float>("elapsed_time", get_pin_style<float>())
		->behaviour([timer]() { return timer->elapsed(); });
	on_update_node->addOUT<float>("delta_time", get_pin_style<float>())
		->behaviour([timer]() { return timer->delta(); });

	m_editor.rightClickPopUpContent([&](ne::BaseNode* node)
	{
		if (node) {
			if (node == on_start_node ||
				node == on_update_node ||
				node == on_collision_node ||
				node == on_ui_node) {
				return;
			}
			if (im::Selectable("Destroy")) {
				node->destroy();
				im::CloseCurrentPopup();
			}
			return;
		}
		const String filter = gui_input_continuous("Name###NewNode");
		for (const auto& [name, generator] : ui_node_generators) {
			if (!filter.empty() && name.find(filter) == -1)
				continue;

			if (im::Selectable(name.c_str())) {
				m_editor.insertNode(m_editor.screen2grid(im::GetMousePos()), generator());
				im::CloseCurrentPopup();
			}
		}
	});
	m_editor.setScroll({ 150.f, 450.f });
}

void titian::NodeScript::serialize(Serializer* serializer, const void* helper_data) const
{
	Script::serialize(serializer, helper_data);

	serializer->write_float("editor_scale", m_editor.getScale());
	serializer->write_float_array("editor_scroll", (const float*) &m_editor.getScroll(), 2);

	auto& nodes = m_editor.getNodes();
	serializer->write_int("node_count", (int32_t) nodes.size() - 4);

	int counter = 0;
	for (auto& [id, node] : nodes) {
		auto node_ptr = dynamic_cast<const Node*>(node.get());
		if (!node_ptr)
			kl::assert(false, "Each node type needs to be derived from Node!");

		if (node_ptr == on_start_node ||
			node_ptr == on_update_node || 
			node_ptr == on_collision_node || 
			node_ptr == on_ui_node) {
			continue;
		}

		const String name = kl::format("__node_", counter);
		serializer->push_object(name);
		serializer->write_byte_array("uid", &id, sizeof(id));
		node_ptr->serialize(serializer, helper_data);
		serializer->pop_object();
		counter += 1;
	}

	auto& links = m_editor.getLinks();
	serializer->write_int("link_count", (int32_t) links.size());

	counter = 0;
	for (auto& link : links) {
		const auto link_ptr = link.lock();
		const auto left_parent = link_ptr->left()->getParent()->getUID();
		const auto right_parent = link_ptr->right()->getParent()->getUID();
		const auto& left_name = link_ptr->left()->getName();
		const auto& right_name = link_ptr->right()->getName();

		const String name = kl::format("__link_", counter);
		serializer->push_object(name);
		serializer->write_byte_array("left_parent", &left_parent, sizeof(left_parent));
		serializer->write_byte_array("right_parent", &right_parent, sizeof(right_parent));
		serializer->write_string("left_name", left_name);
		serializer->write_string("right_name", right_name);
		serializer->pop_object();
		counter += 1;
	}
}

void titian::NodeScript::deserialize(const Serializer* serializer, const void* helper_data)
{
	Script::deserialize(serializer, helper_data);

	float editor_scale = 0.0f;
	serializer->read_float("editor_scale", editor_scale);
	m_editor.setScale(editor_scale);

	ImVec2 editor_scroll = {};
	serializer->read_float_array("editor_scroll", (float*) &editor_scroll, 2);
	m_editor.setScroll(editor_scroll);

	int node_count = 0;
	serializer->read_int("node_count", node_count);

	for (int i = 0; i < node_count; i++) {
		const String name = kl::format("__node_", i);
		serializer->load_object(name);

		ne::NodeUID id{};
		serializer->read_byte_array("uid", &id, sizeof(id));

		String node_type;
		serializer->read_string("node_type", node_type);

		std::shared_ptr<Node> node;
		for (auto& [type, generator] : serial_node_generators) {
			if (node_type == type) {
				node = generator();
				break;
			}
		}
		kl::assert((bool) node, "Unknown node type: ", node_type);

		node->deserialize(serializer, helper_data);
		m_editor.insertNode(node);

		serializer->unload_object();
	}

	int link_count = 0;
	serializer->read_int("link_count", link_count);

	const auto& nodes = m_editor.getNodes();
	for (int i = 0; i < link_count; i++) {
		const String name = kl::format("__link_", i);
		serializer->load_object(name);

		ne::NodeUID left_parent{};
		ne::NodeUID right_parent{};
		serializer->read_byte_array("left_parent", &left_parent, sizeof(left_parent));
		serializer->read_byte_array("right_parent", &right_parent, sizeof(right_parent));

		String left_name;
		String right_name;
		serializer->read_string("left_name", left_name);
		serializer->read_string("right_name", right_name);

		if (nodes.contains(left_parent) && nodes.contains(right_parent)) {
			auto left_pin = nodes.at(left_parent)->outPin(left_name);
			auto right_pin = nodes.at(right_parent)->inPin(right_name);
			if (left_pin && right_pin) {
				right_pin->createLink(left_pin);
			}
		}

		serializer->unload_object();
	}

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
	on_start_node->call();
}

void titian::NodeScript::call_update(Scene* scene)
{
	on_update_node->call();
}

void titian::NodeScript::call_collision(Scene* scene, Entity* first, Entity* second)
{
	on_collision_node->call();
}

void titian::NodeScript::call_ui(Scene* scene)
{
	on_ui_node->call();
}

void titian::NodeScript::update_editor()
{
	m_editor.update();
}
