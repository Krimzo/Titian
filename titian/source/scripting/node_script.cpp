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
		serial_generator_helper<LiteralNode<Int2>>(""),
		serial_generator_helper<LiteralNode<float>>(""),
		serial_generator_helper<LiteralNode<Float2>>(""),
		serial_generator_helper<LiteralNode<Float3>>(""),
		serial_generator_helper<LiteralNode<Float4>>(""),
		serial_generator_helper<LiteralNode<Complex>>(""),
		serial_generator_helper<LiteralNode<Quaternion>>(""),
		serial_generator_helper<LiteralNode<kl::Color>>(""),
		serial_generator_helper<LiteralNode<String>>(""),

		serial_generator_helper<ConstructNode<Int2>>(""),
		serial_generator_helper<ConstructNode<Float2>>(""),
		serial_generator_helper<ConstructNode<Float3>>(""),
		serial_generator_helper<ConstructNode<Float4>>(""),
		serial_generator_helper<ConstructNode<Complex>>(""),
		serial_generator_helper<ConstructNode<Quaternion>>(""),
		serial_generator_helper<ConstructNode<kl::Color>>(""),

		serial_generator_helper<DeconstructNode<Int2>>(""),
		serial_generator_helper<DeconstructNode<Float2>>(""),
		serial_generator_helper<DeconstructNode<Float3>>(""),
		serial_generator_helper<DeconstructNode<Float4>>(""),
		serial_generator_helper<DeconstructNode<Complex>>(""),
		serial_generator_helper<DeconstructNode<Quaternion>>(""),
		serial_generator_helper<DeconstructNode<kl::Color>>(""),

		serial_generator_helper<CastNode<void*, String>>(""),

		serial_generator_helper<CastNode<bool, int32_t>>(""),
		serial_generator_helper<CastNode<bool, float>>(""),
		serial_generator_helper<CastNode<bool, String>>(""),

		serial_generator_helper<CastNode<int32_t, bool>>(""),
		serial_generator_helper<CastNode<int32_t, float>>(""),
		serial_generator_helper<CastNode<int32_t, String>>(""),

		serial_generator_helper<CastNode<Int2, Float2>>(""),
		serial_generator_helper<CastNode<Int2, String>>(""),

		serial_generator_helper<CastNode<float, bool>>(""),
		serial_generator_helper<CastNode<float, int32_t>>(""),
		serial_generator_helper<CastNode<float, String>>(""),

		serial_generator_helper<CastNode<Float2, Int2>>(""),
		serial_generator_helper<CastNode<Float2, Complex>>(""),
		serial_generator_helper<CastNode<Float2, String>>(""),

		serial_generator_helper<CastNode<Float3, Quaternion>>(""),
		serial_generator_helper<CastNode<Float3, kl::Color>>(""),
		serial_generator_helper<CastNode<Float3, String>>(""),

		serial_generator_helper<CastNode<Float4, Quaternion>>(""),
		serial_generator_helper<CastNode<Float4, kl::Color>>(""),
		serial_generator_helper<CastNode<Float4, String>>(""),

		serial_generator_helper<CastNode<Complex, Float2>>(""),
		serial_generator_helper<CastNode<Complex, String>>(""),

		serial_generator_helper<CastNode<Quaternion, Float3>>(""),
		serial_generator_helper<CastNode<Quaternion, Float4>>(""),
		serial_generator_helper<CastNode<Quaternion, String>>(""),

		serial_generator_helper<CastNode<kl::Color, Float3>>(""),
		serial_generator_helper<CastNode<kl::Color, Float4>>(""),

		serial_generator_helper<CastNode<String, bool>>(""),
		serial_generator_helper<CastNode<String, int32_t>>(""),
		serial_generator_helper<CastNode<String, float>>(""),

		serial_generator_helper<CompareNode<bool>>(""),
		serial_generator_helper<CompareNode<int32_t>>(""),
		serial_generator_helper<CompareNode<float>>(""),
		serial_generator_helper<CompareNode<String>>(""),

		serial_generator_helper<LogicNotNode>(),
		serial_generator_helper<LogicAndNode>(),
		serial_generator_helper<LogicOrNode>(),

		serial_generator_helper<OperatorPlusNode<int32_t>>(),
		serial_generator_helper<OperatorPlusNode<Int2>>(),
		serial_generator_helper<OperatorPlusNode<float>>(),
		serial_generator_helper<OperatorPlusNode<Float2>>(),
		serial_generator_helper<OperatorPlusNode<Float3>>(),
		serial_generator_helper<OperatorPlusNode<Float4>>(),
		serial_generator_helper<OperatorPlusNode<Complex>>(),
		serial_generator_helper<OperatorPlusNode<Quaternion>>(),
		serial_generator_helper<OperatorPlusNode<String>>(),

		serial_generator_helper<OperatorMinusNode<int32_t>>(),
		serial_generator_helper<OperatorMinusNode<Int2>>(),
		serial_generator_helper<OperatorMinusNode<float>>(),
		serial_generator_helper<OperatorMinusNode<Float2>>(),
		serial_generator_helper<OperatorMinusNode<Float3>>(),
		serial_generator_helper<OperatorMinusNode<Float4>>(),
		serial_generator_helper<OperatorMinusNode<Complex>>(),
		serial_generator_helper<OperatorMinusNode<Quaternion>>(),

		serial_generator_helper<OperatorTimesNode<int32_t>>(),
		serial_generator_helper<OperatorTimesNode<Int2>>(),
		serial_generator_helper<OperatorTimesNode<float>>(),
		serial_generator_helper<OperatorTimesNode<Float2>>(),
		serial_generator_helper<OperatorTimesNode<Float3>>(),
		serial_generator_helper<OperatorTimesNode<Float4>>(),
		serial_generator_helper<OperatorTimesNode<Complex>>(),
		serial_generator_helper<OperatorTimesNode<Quaternion>>(),

		serial_generator_helper<OperatorDivideNode<int32_t>>(),
		serial_generator_helper<OperatorDivideNode<Int2>>(),
		serial_generator_helper<OperatorDivideNode<float>>(),
		serial_generator_helper<OperatorDivideNode<Float2>>(),
		serial_generator_helper<OperatorDivideNode<Float3>>(),
		serial_generator_helper<OperatorDivideNode<Float4>>(),

		serial_generator_helper<OperatorPowerNode<int32_t>>(),
		serial_generator_helper<OperatorPowerNode<float>>(),

		serial_generator_helper<OperatorModuloNode<int32_t>>(),
		serial_generator_helper<OperatorModuloNode<float>>(),

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
		ui_generator_helper<LiteralNode<Int2>>("Int2", "Int2"),
		ui_generator_helper<LiteralNode<float>>("Float", "Float"),
		ui_generator_helper<LiteralNode<Float2>>("Float2", "Float2"),
		ui_generator_helper<LiteralNode<Float3>>("Float3", "Float3"),
		ui_generator_helper<LiteralNode<Float4>>("Float4", "Float4"),
		ui_generator_helper<LiteralNode<Complex>>("Complex", "Complex"),
		ui_generator_helper<LiteralNode<Quaternion>>("Quaternion", "Quaternion"),
		ui_generator_helper<LiteralNode<kl::Color>>("Color", "Color"),
		ui_generator_helper<LiteralNode<String>>("String", "String"),

		ui_generator_helper<ConstructNode<Int2>>("Construct Int2", "Construct Int2"),
		ui_generator_helper<ConstructNode<Float2>>("Construct Float2", "Construct Float2"),
		ui_generator_helper<ConstructNode<Float3>>("Construct Float3", "Construct Float3"),
		ui_generator_helper<ConstructNode<Float4>>("Construct Float4", "Construct Float4"),
		ui_generator_helper<ConstructNode<Complex>>("Construct Complex", "Construct Complex"),
		ui_generator_helper<ConstructNode<Quaternion>>("Construct Quaternion", "Construct Quaternion"),
		ui_generator_helper<ConstructNode<kl::Color>>("Construct Color", "Construct Color"),

		ui_generator_helper<DeconstructNode<Int2>>("Deconstruct Int2", "Deconstruct Int2"),
		ui_generator_helper<DeconstructNode<Float2>>("Deconstruct Float2", "Deconstruct Float2"),
		ui_generator_helper<DeconstructNode<Float3>>("Deconstruct Float3", "Deconstruct Float3"),
		ui_generator_helper<DeconstructNode<Float4>>("Deconstruct Float4", "Deconstruct Float4"),
		ui_generator_helper<DeconstructNode<Complex>>("Deconstruct Complex", "Deconstruct Complex"),
		ui_generator_helper<DeconstructNode<Quaternion>>("Deconstruct Quaternion", "Deconstruct Quaternion"),
		ui_generator_helper<DeconstructNode<kl::Color>>("Deconstruct Color", "Deconstruct Color"),

		ui_generator_helper<CastNode<void*, String>>("Pointer -> String", "Pointer -> String"),

		ui_generator_helper<CastNode<bool, int32_t>>("Bool -> Int", "Bool -> Int"),
		ui_generator_helper<CastNode<bool, float>>("Bool -> Float", "Bool -> Float"),
		ui_generator_helper<CastNode<bool, String>>("Bool -> String", "Bool -> String"),

		ui_generator_helper<CastNode<int32_t, bool>>("Int -> Bool", "Int -> Bool"),
		ui_generator_helper<CastNode<int32_t, float>>("Int -> Float", "Int -> Float"),
		ui_generator_helper<CastNode<int32_t, String>>("Int -> String", "Int -> String"),

		ui_generator_helper<CastNode<Int2, Float2>>("Int2 -> Float2", "Int2 -> Float2"),
		ui_generator_helper<CastNode<Int2, String>>("Int2 -> String", "Int2 -> String"),

		ui_generator_helper<CastNode<float, bool>>("Float -> Bool", "Float -> Bool"),
		ui_generator_helper<CastNode<float, int32_t>>("Float -> Int", "Float -> Int"),
		ui_generator_helper<CastNode<float, String>>("Float -> String", "Float -> String"),

		ui_generator_helper<CastNode<Float2, Int2>>("Float2 -> Int2", "Float2 -> Int2"),
		ui_generator_helper<CastNode<Float2, Complex>>("Float2 -> Complex", "Float2 -> Complex"),
		ui_generator_helper<CastNode<Float2, String>>("Float2 -> String", "Float2 -> String"),

		ui_generator_helper<CastNode<Float3, Quaternion>>("Float3 -> Quaternion", "Float3 -> Quaternion"),
		ui_generator_helper<CastNode<Float3, kl::Color>>("Float3 -> Color", "Float3 -> Color"),
		ui_generator_helper<CastNode<Float3, String>>("Float3 -> String", "Float3 -> String"),

		ui_generator_helper<CastNode<Float4, Quaternion>>("Float4 -> Quaternion", "Float4 -> Quaternion"),
		ui_generator_helper<CastNode<Float4, kl::Color>>("Float4 -> Color", "Float4 -> Color"),
		ui_generator_helper<CastNode<Float4, String>>("Float4 -> String", "Float4 -> String"),

		ui_generator_helper<CastNode<Complex, Float2>>("Complex -> Float2", "Complex -> Float2"),
		ui_generator_helper<CastNode<Complex, String>>("Complex -> String", "Complex -> String"),

		ui_generator_helper<CastNode<Quaternion, Float3>>("Quaternion -> Float3", "Quaternion -> Float3"),
		ui_generator_helper<CastNode<Quaternion, Float4>>("Quaternion -> Float4", "Quaternion -> Float4"),
		ui_generator_helper<CastNode<Quaternion, String>>("Quaternion -> String", "Quaternion -> String"),

		ui_generator_helper<CastNode<kl::Color, Float3>>("Color -> Float3", "Color -> Float3"),
		ui_generator_helper<CastNode<kl::Color, Float4>>("Color -> Float4", "Color -> Float4"),

		ui_generator_helper<CastNode<String, bool>>("String -> Bool", "String -> Bool"),
		ui_generator_helper<CastNode<String, int32_t>>("String -> Int", "String -> Int"),
		ui_generator_helper<CastNode<String, float>>("String -> Float", "String -> Float"),

		ui_generator_helper<CompareNode<bool>>("Compare Bool",  "Compare Bool"),
		ui_generator_helper<CompareNode<int32_t>>("Compare Int", "Compare Int"),
		ui_generator_helper<CompareNode<float>>("Compare Float",  "Compare Float"),
		ui_generator_helper<CompareNode<String>>("Compare String",  "Compare String"),

		ui_generator_helper<LogicNotNode>("Not"),
		ui_generator_helper<LogicAndNode>("And"),
		ui_generator_helper<LogicOrNode>("Or"),

		ui_generator_helper<OperatorPlusNode<int32_t>>("Plus Int"),
		ui_generator_helper<OperatorPlusNode<Int2>>("Plus Int2"),
		ui_generator_helper<OperatorPlusNode<float>>("Plus Float"),
		ui_generator_helper<OperatorPlusNode<Float2>>("Plus Float2"),
		ui_generator_helper<OperatorPlusNode<Float3>>("Plus Float3"),
		ui_generator_helper<OperatorPlusNode<Float4>>("Plus Float4"),
		ui_generator_helper<OperatorPlusNode<Complex>>("Plus Complex"),
		ui_generator_helper<OperatorPlusNode<Quaternion>>("Plus Quaternion"),
		ui_generator_helper<OperatorPlusNode<String>>("Plus String"),

		ui_generator_helper<OperatorMinusNode<int32_t>>("Minus Int"),
		ui_generator_helper<OperatorMinusNode<Int2>>("Minus Int2"),
		ui_generator_helper<OperatorMinusNode<float>>("Minus Float"),
		ui_generator_helper<OperatorMinusNode<Float2>>("Minus Float2"),
		ui_generator_helper<OperatorMinusNode<Float3>>("Minus Float3"),
		ui_generator_helper<OperatorMinusNode<Float4>>("Minus Float4"),
		ui_generator_helper<OperatorMinusNode<Complex>>("Minus Complex"),
		ui_generator_helper<OperatorMinusNode<Quaternion>>("Minus Quaternion"),

		ui_generator_helper<OperatorTimesNode<int32_t>>("Times Int"),
		ui_generator_helper<OperatorTimesNode<Int2>>("Times Int2"),
		ui_generator_helper<OperatorTimesNode<float>>("Times Float"),
		ui_generator_helper<OperatorTimesNode<Float2>>("Times Float2"),
		ui_generator_helper<OperatorTimesNode<Float3>>("Times Float3"),
		ui_generator_helper<OperatorTimesNode<Float4>>("Times Float4"),
		ui_generator_helper<OperatorTimesNode<Complex>>("Times Complex"),
		ui_generator_helper<OperatorTimesNode<Quaternion>>("Times Quaternion"),

		ui_generator_helper<OperatorDivideNode<int32_t>>("Divide Int"),
		ui_generator_helper<OperatorDivideNode<Int2>>("Divide Int2"),
		ui_generator_helper<OperatorDivideNode<float>>("Divide Float"),
		ui_generator_helper<OperatorDivideNode<Float2>>("Divide Float2"),
		ui_generator_helper<OperatorDivideNode<Float3>>("Divide Float3"),
		ui_generator_helper<OperatorDivideNode<Float4>>("Divide Float4"),

		ui_generator_helper<OperatorPowerNode<int32_t>>("Power Int"),
		ui_generator_helper<OperatorPowerNode<float>>("Power Float"),

		ui_generator_helper<OperatorModuloNode<int32_t>>("Modulo Int"),
		ui_generator_helper<OperatorModuloNode<float>>("Modulo Float"),

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

	on_collision_node->addOUT<void*>("first_entity", get_pin_style<void*>())
		->behaviour([this]() { return on_collision_node->user_data + 0; });
	on_collision_node->addOUT<void*>("second_entity", get_pin_style<void*>())
		->behaviour([this]() { return on_collision_node->user_data + 8; });

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
	for (auto& [uid, node] : nodes) {
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
		serializer->write_byte_array("uid", &uid, sizeof(uid));
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

		ne::NodeUID uid{};
		serializer->read_byte_array("uid", &uid, sizeof(uid));

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
		node->setUID(uid);
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
	*reinterpret_cast<Entity**>(on_collision_node->user_data + 0) = first;
	*reinterpret_cast<Entity**>(on_collision_node->user_data + 8) = second;
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
