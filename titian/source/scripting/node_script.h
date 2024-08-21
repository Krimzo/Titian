#pragma once

#include "scripting/script.h"
#include "scene/scene.h"


namespace ne = ImFlow;

namespace titian {
	struct FlowNode;
}

namespace titian {
	class NodeScript : public Script
	{
	public:
		FlowNode* on_start_node = nullptr;
		FlowNode* on_update_node = nullptr;
		FlowNode* on_collision_node = nullptr;
		FlowNode* on_ui_node = nullptr;

		struct VarInfo
		{
			using ValueType = std::variant<
				void*,
				bool,
				int32_t,
				Int2,
				float,
				Float2,
				Float3,
				Float4,
				Complex,
				Quaternion,
				Color,
				String>;

			bool global = false;
			ValueType value = {};

			template<typename T>
			T& get()
			{
				return std::get<T>(value);
			}

			template<typename T>
			T& store(const T& data)
			{
				return value.emplace<T>(data);
			}
		};

		Map<String, VarInfo> var_storage;

		NodeScript();

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		bool is_valid() const override;
		void reload() override;

		void call_start(Scene* scene) override;
		void call_update(Scene* scene) override;
		void call_collision(Scene* scene, Entity* first, Entity* second) override;
		void call_ui(Scene* scene) override;

		void update_editor();

	private:
		ne::ImNodeFlow m_editor;
	};
}

// base node
namespace titian {
	struct Node : ne::BaseNode, Serializable
	{
		NodeScript* parent = nullptr;
		byte user_data[16] = {};

		inline Node(NodeScript* parent, const String& title, const std::shared_ptr<ne::NodeStyle>& style)
			: parent(parent)
		{
			setTitle(title);
			setStyle(style);
		}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			serializer->write_string("node_type", typeid(self).name()); // must be read from outside

			serializer->write_byte_array("user_data", user_data, sizeof(user_data));
			serializer->write_string("title", getName());
			serializer->write_float_array("position", (const float*) &getPos(), 2);

			auto& style = *getStyle();
			serializer->write_byte_array("style", &style, sizeof(style));
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			serializer->read_byte_array("user_data", user_data, sizeof(user_data));
			
			String title;
			serializer->read_string("title", title);
			setTitle(title);

			ImVec2 position{};
			serializer->read_float_array("position", (float*) &position, 2);
			setPos(position);

			std::shared_ptr style = ne::NodeStyle::red();
			serializer->read_byte_array("style", style.get(), sizeof(*style));
			setStyle(style);
		}

		template<typename T>
		const T& get_value(const String& id)
		{
			ne::Pin* ptr = this->inPin(id);
			ne::InPin<T>* in_ptr = reinterpret_cast<ne::InPin<T>*>(ptr);
			return in_ptr->val();
		}

		template<typename From, typename To>
		To get_casted_value(const String& id)
		{
			return (To) get_value<From>(id);
		}
	};
}

// flow node
namespace titian {
	struct FlowNode : Node
	{
		bool has_input = false;
		bool has_output = false;

		inline FlowNode(NodeScript* parent, const String& title, bool has_input, bool has_output, std::shared_ptr<ne::NodeStyle> style = ne::NodeStyle::white())
			: Node(parent, title, style), has_input(has_input), has_output(has_output)
		{
			if (has_input) {
				addIN<FlowNode*>("in_flow");
			}
			if (has_output) {
				addOUT<FlowNode*>("out_flow")->behaviour([this] { return this; });
			}
		}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			Node::serialize(serializer, helper_data);

			serializer->write_bool("has_input", has_input);
			serializer->write_bool("has_output", has_output);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			Node::deserialize(serializer, helper_data);

			bool saved_has_input = false;
			bool saved_has_ouput = false;
			serializer->read_bool("has_input", saved_has_input);
			serializer->read_bool("has_output", saved_has_ouput);

			if (!has_input && saved_has_input) {
				has_input = true;
				addIN<FlowNode*>("in_flow");
			}
			if (!has_output && saved_has_ouput) {
				has_output = true;
				addOUT<FlowNode*>("out_flow")->behaviour([this] { return this; });
			}
		}

		virtual void call()
		{
			call_next();
		}

	protected:
		inline void call_next(const StringView& pin_name = "out_flow")
		{
			auto pin = this->outPin(pin_name.data());
			if (!pin)
				return;

			auto link = pin->getLink().lock();
			if (!link)
				return;

			auto next_node = dynamic_cast<FlowNode*>(link->right()->getParent());
			if (!next_node)
				return;

			next_node->call();
		}
	};
}

// literal node
namespace titian {
	template<typename T>
	struct LiteralNode : Node
	{
		T value = {};

		inline LiteralNode(NodeScript* parent, const String& title)
			: Node(parent, title, ne::NodeStyle::green())
		{
			addOUT<T>("result")->behaviour([this] { return this->value; });
		}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			Node::serialize(serializer, helper_data);

			if constexpr (std::is_same_v<T, bool>) {
				serializer->write_bool("value", value);
			}
			else if constexpr (std::is_same_v<T, int32_t>) {
				serializer->write_int("value", value);
			}
			else if constexpr (std::is_same_v<T, float>) {
				serializer->write_float("value", value);
			}
			else if constexpr (std::is_same_v<T, String>) {
				serializer->write_string("value", value);
			}
			else if constexpr (std::is_same_v<T, Int2>) {
				serializer->write_int_array("value", value, 2);
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				serializer->write_float_array("value", value, 2);
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				serializer->write_float_array("value", value, 3);
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				serializer->write_float_array("value", value, 4);
			}
			else if constexpr (std::is_same_v<T, Complex>) {
				serializer->write_float_array("value", value, 2);
			}
			else if constexpr (std::is_same_v<T, Quaternion>) {
				serializer->write_float_array("value", value, 4);
			}
			else if constexpr (std::is_same_v<T, Color>) {
				serializer->write_byte_array("value", &value, 4);
			}
			else {
				static_assert(false, "Unknown serialize literal node type");
			}
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			Node::deserialize(serializer, helper_data);

			if constexpr (std::is_same_v<T, bool>) {
				serializer->read_bool("value", value);
			}
			else if constexpr (std::is_same_v<T, int32_t>) {
				serializer->read_int("value", value);
			}
			else if constexpr (std::is_same_v<T, float>) {
				serializer->read_float("value", value);
			}
			else if constexpr (std::is_same_v<T, String>) {
				serializer->read_string("value", value);
			}
			else if constexpr (std::is_same_v<T, Int2>) {
				serializer->read_int_array("value", value, 2);
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				serializer->read_float_array("value", value, 2);
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				serializer->read_float_array("value", value, 3);
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				serializer->read_float_array("value", value, 4);
			}
			else if constexpr (std::is_same_v<T, Complex>) {
				serializer->read_float_array("value", value, 2);
			}
			else if constexpr (std::is_same_v<T, Quaternion>) {
				serializer->read_float_array("value", value, 4);
			}
			else if constexpr (std::is_same_v<T, Color>) {
				serializer->read_byte_array("value", &value, 4);
			}
			else {
				static_assert(false, "Unknown deserialize literal node type");
			}
		}

		void draw() override
		{
			im::SetNextItemWidth(100.0f);
			if constexpr (std::is_same_v<T, bool>) {
				im::Checkbox("##value", &value);
			}
			else if constexpr (std::is_same_v<T, int32_t>) {
				im::InputInt("##value", &value);
			}
			else if constexpr (std::is_same_v<T, float>) {
				im::InputFloat("##value", &value);
			}
			else if constexpr (std::is_same_v<T, String>) {
				im::InputText("##value", &value);
			}
			else if constexpr (std::is_same_v<T, Int2>) {
				im::InputInt2("##value", value);
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				im::InputFloat2("##value", value);
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				im::InputFloat3("##value", value);
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				im::InputFloat4("##value", value);
			}
			else if constexpr (std::is_same_v<T, Complex>) {
				im::InputFloat2("##value", value);
			}
			else if constexpr (std::is_same_v<T, Quaternion>) {
				im::InputFloat4("##value", value);
			}
			else if constexpr (std::is_same_v<T, Color>) {
				Float4 temp = value;
				if (im::ColorEdit4("##value", temp)) {
					value = temp;
				}
			}
			else {
				static_assert(false, "Unknown draw literal node type");
			}
		}
	};
}

// variable node
namespace titian {
	template<typename T>
	struct VariableNode : FlowNode, kl::NoCopy, kl::NoMove
	{
		using VarInfo = NodeScript::VarInfo;

		String name;
		VarInfo* var_ptr = nullptr;

		inline VariableNode(NodeScript* parent, const String& title, const String& var_name)
			: FlowNode(parent, title, true, true, ne::NodeStyle::cyan())
		{
			rename(var_name);
			addIN<T>("write");
			addOUT<T>("read")->behaviour([this]()
			{
				return var_ptr->get<T>();
			});
		}

		~VariableNode() override
		{
			storage().erase(name);
		}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			FlowNode::serialize(serializer, helper_data);
			serializer->write_string("var_name", name);
			serializer->write_bool("global", var_ptr->global);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			FlowNode::deserialize(serializer, helper_data);
			String temp_name;
			serializer->read_string("var_name", temp_name);
			rename(temp_name);
			serializer->read_bool("global", var_ptr->global);
		}

		void draw() override
		{
			String temp_name = name;
			im::SetNextItemWidth(100.0f);
			if (im::InputText("##name", &temp_name)) {
				rename(temp_name);
			}
			im::Checkbox("global", &var_ptr->global);
		}

		void call() override
		{
			var_ptr->store<T>(get_value<T>("write"));
			call_next();
		}

		inline void rename(const String& new_name)
		{
			VarInfo old_info;
			if (var_ptr) {
				old_info = *var_ptr;
			}
			else {
				old_info.store<T>(T{});
			}
			storage().erase(name);
			name = !new_name.empty() ? new_name : kl::random::gen_string(10);
			for (int i = 1; storage().contains(name); i++) {
				name = kl::format(new_name, '_', i);
			}
			var_ptr = &storage()[name];
			*var_ptr = old_info;
		}

	private:
		inline Map<String, VarInfo>& storage() const
		{
			return parent->var_storage;
		}
	};
}

// construct/deconstruct nodes
namespace titian {
	template<typename T>
	struct ConstructNode : Node
	{
		inline ConstructNode(NodeScript* parent, const String& title)
			: Node(parent, title, ne::NodeStyle::teal())
		{
			if constexpr (std::is_same_v<T, Int2>) {
				addIN<int32_t>("x");
				addIN<int32_t>("y");
				addOUT<T>("out")->behaviour([this]()
				{
					return T{ get_value<int32_t>("x"), get_value<int32_t>("y") };
				});
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				addIN<float>("x");
				addIN<float>("y");
				addOUT<T>("out")->behaviour([this]()
				{
					return T{ get_value<float>("x"), get_value<float>("y") };
				});
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				addIN<float>("x");
				addIN<float>("y");
				addIN<float>("z");
				addOUT<T>("out")->behaviour([this]()
				{
					return T{ get_value<float>("x"), get_value<float>("y"), get_value<float>("z") };
				});
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				addIN<float>("x");
				addIN<float>("y");
				addIN<float>("z");
				addIN<float>("w");
				addOUT<T>("out")->behaviour([this]()
				{
					return T{ get_value<float>("x"), get_value<float>("y"), get_value<float>("z"), get_value<float>("w") };
				});
			}
			else if constexpr (std::is_same_v<T, Complex>) {
				addIN<float>("r");
				addIN<float>("i");
				addOUT<T>("out")->behaviour([this]()
				{
					return T{ get_value<float>("r"), get_value<float>("i") };
				});
			}
			else if constexpr (std::is_same_v<T, Quaternion>) {
				addIN<float>("w");
				addIN<float>("x");
				addIN<float>("y");
				addIN<float>("z");
				addOUT<T>("out")->behaviour([this]()
				{
					return T{ get_value<float>("w"), get_value<float>("x"), get_value<float>("y"), get_value<float>("z") };
				});
			}
			else if constexpr (std::is_same_v<T, Color>) {
				addIN<int32_t>("r");
				addIN<int32_t>("g");
				addIN<int32_t>("b");
				addIN<int32_t>("a");
				addOUT<T>("out")->behaviour([this]()
				{
					return T{
						(byte) get_value<int32_t>("r"),
						(byte) get_value<int32_t>("g"),
						(byte) get_value<int32_t>("b"),
						(byte) get_value<int32_t>("a"),
					};
				});
			}
			else {
				static_assert(false, "Unknown construct node type");
			}
		}
	};

	template<typename T>
	struct DeconstructNode : Node
	{
		inline DeconstructNode(NodeScript* parent, const String& title)
			: Node(parent, title, ne::NodeStyle::teal())
		{
			if constexpr (std::is_same_v<T, Int2>) {
				addIN<T>("in");
				addOUT<int32_t>("x")->behaviour([this]()
				{
					return get_value<T>("in").x;
				});
				addOUT<int32_t>("y")->behaviour([this]()
				{
					return get_value<T>("in").y;
				});
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				addIN<T>("in");
				addOUT<float>("x")->behaviour([this]()
				{
					return get_value<T>("in").x;
				});
				addOUT<float>("y")->behaviour([this]()
				{
					return get_value<T>("in").y;
				});
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				addIN<T>("in");
				addOUT<float>("x")->behaviour([this]()
				{
					return get_value<T>("in").x;
				});
				addOUT<float>("y")->behaviour([this]()
				{
					return get_value<T>("in").y;
				});
				addOUT<float>("z")->behaviour([this]()
				{
					return get_value<T>("in").z;
				});
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				addIN<T>("in");
				addOUT<float>("x")->behaviour([this]()
				{
					return get_value<T>("in").x;
				});
				addOUT<float>("y")->behaviour([this]()
				{
					return get_value<T>("in").y;
				});
				addOUT<float>("z")->behaviour([this]()
				{
					return get_value<T>("in").z;
				});
				addOUT<float>("w")->behaviour([this]()
				{
					return get_value<T>("in").w;
				});
			}
			else if constexpr (std::is_same_v<T, Complex>) {
				addIN<T>("in");
				addOUT<float>("r")->behaviour([this]()
				{
					return get_value<T>("in").r;
				});
				addOUT<float>("i")->behaviour([this]()
				{
					return get_value<T>("in").i;
				});
			}
			else if constexpr (std::is_same_v<T, Quaternion>) {
				addIN<T>("in");
				addOUT<float>("w")->behaviour([this]()
				{
					return get_value<T>("in").w;
				});
				addOUT<float>("x")->behaviour([this]()
				{
					return get_value<T>("in").x;
				});
				addOUT<float>("y")->behaviour([this]()
				{
					return get_value<T>("in").y;
				});
				addOUT<float>("z")->behaviour([this]()
				{
					return get_value<T>("in").z;
				});
			}
			else if constexpr (std::is_same_v<T, Color>) {
				addIN<T>("in");
				addOUT<int32_t>("r")->behaviour([this]()
				{
					return get_value<T>("in").r;
				});
				addOUT<int32_t>("g")->behaviour([this]()
				{
					return get_value<T>("in").g;
				});
				addOUT<int32_t>("b")->behaviour([this]()
				{
					return get_value<T>("in").b;
				});
				addOUT<int32_t>("a")->behaviour([this]()
				{
					return get_value<T>("in").a;
				});
			}
			else {
				static_assert(false, "Unknown deconstruct node type");
			}
		}
	};
}

// cast node
namespace titian {
	template<typename From, typename To>
	struct CastNode : Node
	{
		inline CastNode(NodeScript* parent, const String& title)
			: Node(parent, title, ne::NodeStyle::orange())
		{
			addIN<From>("from");
			addOUT<To>("to")->behaviour([this]
			{
				auto& from = get_value<From>("from");
				if constexpr (std::is_same_v<From, String>) {
					if constexpr (std::is_same_v<To, bool>) {
						return from == "true";
					}
					else if constexpr (std::is_same_v<To, int32_t>) {
						return (int32_t) kl::parse_int(from).value_or(0);
					}
					else if constexpr (std::is_same_v<To, float>) {
						return (float) kl::parse_float(from).value_or(0.0);
					}
					else {
						static_assert(false, "Unkown cast node from String to T");
					}
				}
				else if constexpr (std::is_same_v<To, String>) {
					return kl::format(std::boolalpha, std::fixed, from);
				}
				else {
					return static_cast<To>(from);
				}
			});
		}
	};
}

// compare node
namespace titian {
	template<typename T>
	struct CompareNode : Node
	{
		inline CompareNode(NodeScript* parent, const String& title)
			: Node(parent, title, ne::NodeStyle::yellow())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<bool>("less")->behaviour([this]
			{
				return get_value<T>("left") < get_value<T>("right");
			});
			addOUT<bool>("equal")->behaviour([this]
			{
				return get_value<T>("left") == get_value<T>("right");
			});
			addOUT<bool>("greater")->behaviour([this]
			{
				return get_value<T>("left") > get_value<T>("right");
			});
		}
	};
}

// logic nodes
namespace titian {
	struct LogicNotNode : Node
	{
		inline LogicNotNode(NodeScript* parent)
			: Node(parent, "Not", ne::NodeStyle::pink())
		{
			addIN<bool>("in");
			addOUT<bool>("out")->behaviour([this]
			{
				return !get_value<bool>("in");
			});
		}
	};

	struct LogicAndNode : Node
	{
		inline LogicAndNode(NodeScript* parent)
			: Node(parent, "And", ne::NodeStyle::pink())
		{
			addIN<bool>("left");
			addIN<bool>("right");
			addOUT<bool>("out")->behaviour([this]
			{
				return get_value<bool>("left") && get_value<bool>("right");
			});
		}
	};

	struct LogicOrNode : Node
	{
		inline LogicOrNode(NodeScript* parent)
			: Node(parent, "Or", ne::NodeStyle::pink())
		{
			addIN<bool>("left");
			addIN<bool>("right");
			addOUT<bool>("out")->behaviour([this]
			{
				return get_value<bool>("left") || get_value<bool>("right");
			});
		}
	};
}

// operator nodes
namespace titian {
	template<typename T>
	struct OperatorPlusNode : Node
	{
		inline OperatorPlusNode(NodeScript* parent)
			: Node(parent, "Plus", ne::NodeStyle::blue())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<T>("out")->behaviour([this]
			{
				return get_value<T>("left") + get_value<T>("right");
			});
		}
	};

	template<typename T>
	struct OperatorMinusNode : Node
	{
		inline OperatorMinusNode(NodeScript* parent)
			: Node(parent, "Minus", ne::NodeStyle::blue())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<T>("out")->behaviour([this]
			{
				return get_value<T>("left") - get_value<T>("right");
			});
		}
	};

	template<typename T>
	struct OperatorTimesNode : Node
	{
		inline OperatorTimesNode(NodeScript* parent)
			: Node(parent, "Times", ne::NodeStyle::blue())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<T>("out")->behaviour([this]
			{
				return get_value<T>("left") * get_value<T>("right");
			});
		}
	};

	template<typename T>
	struct OperatorDivideNode : Node
	{
		inline OperatorDivideNode(NodeScript* parent)
			: Node(parent, "Divide", ne::NodeStyle::blue())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<T>("out")->behaviour([this]
			{
				return get_value<T>("left") / get_value<T>("right");
			});
		}
	};

	template<typename T>
	struct OperatorPowerNode : Node
	{
		inline OperatorPowerNode(NodeScript* parent)
			: Node(parent, "Power", ne::NodeStyle::blue())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<T>("out")->behaviour([this]
			{
				return helper_pow<T>(get_value<T>("left"), get_value<T>("right"));
			});
		}

	private:
		template<typename T>
		static T helper_pow(T left, T right)
		{
			if constexpr (std::is_same_v<T, int32_t>) {
				return (int32_t) ::pow((double) left, (double) right);
			}
			else if constexpr (std::is_same_v<T, float>) {
				return (float) ::pow(left, right);
			}
			else {
				static_assert(false, "Unsupported helper pow type");
			}
		}
	};

	template<typename T>
	struct OperatorModuloNode : Node
	{
		inline OperatorModuloNode(NodeScript* parent)
			: Node(parent, "Modulo", ne::NodeStyle::blue())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<T>("out")->behaviour([this]
			{
				return helper_mod<T>(get_value<T>("left"), get_value<T>("right"));
			});
		}

	private:
		template<typename T>
		static T helper_mod(T left, T right)
		{
			if constexpr (std::is_same_v<T, int32_t>) {
				return (int32_t) (left % right);
			}
			else if constexpr (std::is_same_v<T, float>) {
				return (float) ::fmod(left, right);
			}
			else {
				static_assert(false, "Unsupported helper mod type");
			}
		}
	};
}

// flow control nodes
namespace titian {
	struct IfNode : FlowNode
	{
		inline IfNode(NodeScript* parent)
			: FlowNode(parent, "If", true, false, ne::NodeStyle::red())
		{
			addIN<bool>("value");
			addOUT<FlowNode*>("if")->behaviour([this] { return this; });
			addOUT<FlowNode*>("else")->behaviour([this] { return this; });
		}

		void call() override
		{
			auto& value = get_value<bool>("value");
			if (value) {
				call_next("if");
			}
			else {
				call_next("else");
			}
		}
	};

	struct WhileNode : FlowNode
	{
		inline WhileNode(NodeScript* parent)
			: FlowNode(parent, "While", true, true, ne::NodeStyle::red())
		{
			addIN<bool>("value");
		}

		void call() override
		{
			while (true) {
				auto& value = get_value<bool>("value");
				if (value) {
					call_next();
				}
				else {
					break;
				}
			}
		}
	};

	struct ForNode : FlowNode
	{
		inline ForNode(NodeScript* parent)
			: FlowNode(parent, "For", true, true, ne::NodeStyle::red())
		{
			addIN<int32_t>("from_incl");
			addIN<int32_t>("to_excl");
			addOUT<int32_t>("i")->behaviour([this]()
			{
				return *reinterpret_cast<int32_t*>(user_data);
			});
		}

		void call() override
		{
			auto& from = get_value<int32_t>("from_incl");
			auto& to = get_value<int32_t>("to_excl");
			for (int32_t i = from; i < to; i++) {
				*reinterpret_cast<int32_t*>(user_data) = i;
				call_next();
			}
		}
	};
}

// getter nodes
namespace titian {
	struct GetSceneNode : Node
	{
		GetSceneNode(NodeScript* parent)
			: Node(parent, "Scene", ne::NodeStyle::purple())
		{
			addIN<String>("mesh_name");
			addOUT<void*>("mesh")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.helper_get_mesh(get_value<String>("mesh_name"));
			});

			addIN<String>("animation_name");
			addOUT<void*>("animation")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.helper_get_animation(get_value<String>("animation_name"));
			});

			addIN<String>("texture_name");
			addOUT<void*>("texture")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.helper_get_texture(get_value<String>("texture_name"));
			});

			addIN<String>("material_name");
			addOUT<void*>("material")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.helper_get_material(get_value<String>("material_name"));
			});

			addIN<String>("shader_name");
			addOUT<void*>("shader")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.helper_get_shader(get_value<String>("shader_name"));
			});

			addIN<String>("entity_name");
			addOUT<void*>("entity")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.helper_get_entity(get_value<String>("entity_name"));
			});
		}
	};

	struct GetMeshNode : Node
	{
		GetMeshNode(NodeScript* parent)
			: Node(parent, "Mesh", ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<int32_t>("topology")->behaviour([this]()
			{
				if (Mesh* ptr = get_casted_value<void*, Mesh*>("ptr")) {
					return ptr->topology;
				}
				return int32_t{};
			});
			addOUT<bool>("wireframe")->behaviour([this]()
			{
				if (Mesh* ptr = get_casted_value<void*, Mesh*>("ptr")) {
					return ptr->render_wireframe;
				}
				return bool{};
			});
		}
	};

	struct GetAnimationNode : Node
	{
		GetAnimationNode(NodeScript* parent)
			: Node(parent, "Animation", ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<int32_t>("type")->behaviour([this]()
			{
				if (Animation* ptr = get_casted_value<void*, Animation*>("ptr")) {
					return (int32_t) ptr->animation_type;
				}
				return int32_t{};
			});
			addOUT<float>("ticks per second")->behaviour([this]()
			{
				if (Animation* ptr = get_casted_value<void*, Animation*>("ptr")) {
					return ptr->ticks_per_second;
				}
				return float{};
			});
			addOUT<float>("duration ticks")->behaviour([this]()
			{
				if (Animation* ptr = get_casted_value<void*, Animation*>("ptr")) {
					return ptr->duration_in_ticks;
				}
				return float{};
			});
		}
	};

	struct GetTextureNode : Node
	{
		GetTextureNode(NodeScript* parent)
			: Node(parent, "Texture", ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<Int2>("size")->behaviour([this]()
			{
				if (Texture* ptr = get_casted_value<void*, Texture*>("ptr")) {
					return ptr->graphics_buffer_size();
				}
				return Int2{};
			});
		}
	};

	struct GetMaterialNode : Node
	{
		GetMaterialNode(NodeScript* parent)
			: Node(parent, "Material", ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<Float4>("color")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->color;
				}
				return Float4{};
			});
			addOUT<float>("texture_blend")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->texture_blend;
				}
				return float{};
			});
			addOUT<float>("reflection_factor")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->reflection_factor;
				}
				return float{};
			});
			addOUT<float>("refraction_factor")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->refraction_factor;
				}
				return float{};
			});
			addOUT<float>("refraction_index")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->refraction_index;
				}
				return float{};
			});
			addOUT<String>("color_map_name")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->color_map_name;
				}
				return String{};
			});
			addOUT<String>("normal_map_name")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->normal_map_name;
				}
				return String{};
			});
			addOUT<String>("roughness_map_name")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->roughness_map_name;
				}
				return String{};
			});
			addOUT<String>("shader_name")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->shader_name;
				}
				return String{};
			});
		}
	};

	struct GetShaderNode : Node
	{
		GetShaderNode(NodeScript* parent)
			: Node(parent, "Shader", ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<int32_t>("type")->behaviour([this]()
			{
				if (Shader* ptr = get_casted_value<void*, Shader*>("ptr")) {
					return (int32_t) ptr->type;
				}
				return int32_t{};
			});
		}
	};

	struct GetEntityNode : Node
	{
		GetEntityNode(NodeScript* parent)
			: Node(parent, "Entity", ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<Float3>("scale")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->scale;
				}
				return Float3{};
			});
			addOUT<Float3>("rotation")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->rotation();
				}
				return Float3{};
			});
			addOUT<Float3>("position")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->position();
				}
				return Float3{};
			});
			addOUT<bool>("is_dynamic")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->is_dynamic();
				}
				return bool{};
			});
			addOUT<bool>("has_gravity")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->has_gravity();
				}
				return bool{};
			});
			addOUT<float>("mass")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->mass();
				}
				return float{};
			});
			addOUT<Float3>("velocity")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->velocity();
				}
				return Float3{};
			});
			addOUT<Float3>("angular")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->angular();
				}
				return Float3{};
			});
			addOUT<String>("animation_name")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->animation_name;
				}
				return String{};
			});
			addOUT<String>("material_name")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->material_name;
				}
				return String{};
			});
			addOUT<String>("collider_mesh_name")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->collider_mesh_name;
				}
				return String{};
			});
			addOUT<bool>("casts_shadows")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->casts_shadows;
				}
				return bool{};
			});
		}
	};
}

// functions
namespace titian {
	struct PrintNode : FlowNode
	{
		inline PrintNode(NodeScript* parent)
			: FlowNode(parent, "Print", true, true, ne::NodeStyle::magenta())
		{
			addIN<String>("value");
		}

		void call() override
		{
			auto& value = get_value<String>("value");
			Logger::log(value);
			call_next();
		}
	};
}
