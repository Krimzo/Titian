#pragma once

#include "scripting/script.h"
#include "scene/scene.h"
#include "ui/ui_funcs.h"


template<typename T, typename = void> struct has_less_operator : std::false_type {};
template<typename T> struct has_less_operator<T, std::void_t<decltype(std::declval<T>() < std::declval<T>())>> : std::true_type {};

template<typename T, typename = void> struct has_equals_operator : std::false_type {};
template<typename T> struct has_equals_operator<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>> : std::true_type {};

template<typename T, typename = void> struct has_greater_operator : std::false_type {};
template<typename T> struct has_greater_operator<T, std::void_t<decltype(std::declval<T>() > std::declval<T>())>> : std::true_type {};

namespace ne = ImFlow;

namespace titian {
	struct FlowNode;
}

namespace titian {
	struct NodeScript : Script
	{
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
				RGB,
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

		StringMap<VarInfo> var_storage;

		NodeScript();

		void serialize(Serializer* serializer) const override;
		void deserialize(const Serializer* serializer) override;

		bool is_valid() const override;
		void reload() override;

		void call_start(Scene* scene) override;
		void call_update(Scene* scene) override;
		void call_collision(Scene* scene, Entity* attacker, Entity* target) override;
		void call_ui(Scene* scene) override;

		void update_editor();

	private:
		ne::ImNodeFlow m_editor;
	};
}

namespace titian {
	struct Node : ne::BaseNode, Serializable
	{
		NodeScript* parent = nullptr;
		byte user_data[16] = {};

		Node(NodeScript* parent, const StringView& title, const std::shared_ptr<ne::NodeStyle>& style = ne::NodeStyle::red())
			: parent(parent)
		{
			setTitle(title);
			setStyle(style);
		}

		void serialize(Serializer* serializer) const override
		{
			serializer->write_string("node_type", typeid(*this).name());

			serializer->write_byte_array("user_data", user_data, sizeof(user_data));
			serializer->write_string("title", getName());
			serializer->write_float_array("position", (const float*) &getPos(), 2);

			auto& style = *getStyle();
			serializer->write_byte_array("style", &style, sizeof(style));
		}

		void deserialize(const Serializer* serializer) override
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

		bool input_connected(const char* uid)
		{
			ne::Pin* ptr = this->inPin(uid);
			return ptr->isConnected();
		}

		template<typename T>
		const T& get_value(const char* uid)
		{
			ne::Pin* ptr = this->inPin(uid);
			ne::InPin<T>* in_ptr = reinterpret_cast<ne::InPin<T>*>(ptr);
			return in_ptr->val();
		}

		template<typename From, typename To>
		To get_casted_value(const char* uid)
		{
			return (To) get_value<From>(uid);
		}
	};
}

namespace titian {
	struct FlowNode : Node
	{
		bool has_input = false;
		bool has_output = false;

		FlowNode(NodeScript* parent, const StringView& title, bool has_input = false, bool has_output = false, std::shared_ptr<ne::NodeStyle> style = ne::NodeStyle::white())
			: Node(parent, title, style), has_input(has_input), has_output(has_output)
		{
			if (has_input) {
				addIN<FlowNode*>("in_flow");
			}
			if (has_output) {
				addOUT<FlowNode*>("out_flow")->behaviour([this] { return this; });
			}
		}

		void serialize(Serializer* serializer) const override
		{
			Node::serialize(serializer);

			serializer->write_bool("has_input", has_input);
			serializer->write_bool("has_output", has_output);
		}

		void deserialize(const Serializer* serializer) override
		{
			Node::deserialize(serializer);

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
		void call_next(const char* pin_name = "out_flow")
		{
			auto pin = this->outPin(pin_name);
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

namespace titian {
	template<typename T>
	struct LiteralNode : Node
	{
		T value = {};

		LiteralNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::green())
		{
			addOUT<T>("result")->behaviour([this] { return this->value; });
		}

		void serialize(Serializer* serializer) const override
		{
			Node::serialize(serializer);

			if constexpr (std::is_same_v<T, bool>) {
				serializer->write_bool("value", value);
			}
			else if constexpr (std::is_same_v<T, int32_t>) {
				serializer->write_int("value", value);
			}
			else if constexpr (std::is_same_v<T, Int2>) {
				serializer->write_int_array("value", &value.x, 2);
			}
			else if constexpr (std::is_same_v<T, float>) {
				serializer->write_float("value", value);
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				serializer->write_float_array("value", &value.x, 2);
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				serializer->write_float_array("value", &value.x, 3);
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				serializer->write_float_array("value", &value.x, 4);
			}
			else if constexpr (std::is_same_v<T, Complex>) {
				serializer->write_float_array("value", &value.r, 2);
			}
			else if constexpr (std::is_same_v<T, Quaternion>) {
				serializer->write_float_array("value", &value.w, 4);
			}
			else if constexpr (std::is_same_v<T, RGB>) {
				serializer->write_byte_array("value", &value, 4);
			}
			else if constexpr (std::is_same_v<T, String>) {
				serializer->write_string("value", value);
			}
			else {
				static_assert(false, "Unknown serialize literal node type");
			}
		}

		void deserialize(const Serializer* serializer) override
		{
			Node::deserialize(serializer);

			if constexpr (std::is_same_v<T, bool>) {
				serializer->read_bool("value", value);
			}
			else if constexpr (std::is_same_v<T, int32_t>) {
				serializer->read_int("value", value);
			}
			else if constexpr (std::is_same_v<T, Int2>) {
				serializer->read_int_array("value", &value.x, 2);
			}
			else if constexpr (std::is_same_v<T, float>) {
				serializer->read_float("value", value);
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				serializer->read_float_array("value", &value.x, 2);
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				serializer->read_float_array("value", &value.x, 3);
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				serializer->read_float_array("value", &value.x, 4);
			}
			else if constexpr (std::is_same_v<T, Complex>) {
				serializer->read_float_array("value", &value.r, 2);
			}
			else if constexpr (std::is_same_v<T, Quaternion>) {
				serializer->read_float_array("value", &value.w, 4);
			}
			else if constexpr (std::is_same_v<T, RGB>) {
				serializer->read_byte_array("value", &value, 4);
			}
			else if constexpr (std::is_same_v<T, String>) {
				serializer->read_string("value", value);
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
			else if constexpr (std::is_same_v<T, Int2>) {
				im::InputInt2("##value", &value.x);
			}
			else if constexpr (std::is_same_v<T, float>) {
				im::InputFloat("##value", &value);
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				im::InputFloat2("##value", &value.x);
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				im::InputFloat3("##value", &value.x);
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				im::InputFloat4("##value", &value.x);
			}
			else if constexpr (std::is_same_v<T, Complex>) {
				im::InputFloat2("##value", &value.r);
			}
			else if constexpr (std::is_same_v<T, Quaternion>) {
				im::InputFloat4("##value", &value.w);
			}
			else if constexpr (std::is_same_v<T, RGB>) {
				Float4 temp = value;
				if (im::ColorEdit4("##value", &temp.x)) {
					value = temp;
				}
			}
			else if constexpr (std::is_same_v<T, String>) {
				im::InputText("##value", &value);
			}
			else {
				static_assert(false, "Unknown draw literal node type");
			}
		}
	};
}

namespace titian {
	template<typename T>
	struct VariableNode : FlowNode, kl::NoCopy, kl::NoMove
	{
		using VarInfo = NodeScript::VarInfo;

		String name;
		VarInfo* var_ptr = nullptr;

		VariableNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::cyan())
		{
			rename("variable");
			addIN<T>("write");
			addOUT<String>("name")->behaviour([this]()
			{
				return name;
			});
			addOUT<T>("read")->behaviour([this]()
			{
				return var_ptr->get<T>();
			});
		}

		~VariableNode() override
		{
			storage().erase(name);
		}

		void serialize(Serializer* serializer) const override
		{
			FlowNode::serialize(serializer);
			serializer->write_string("var_name", name);
			serializer->write_bool("var_global", var_ptr->global);
			serializer->write_byte_array("var_value", &var_ptr->get<T>(), sizeof(T));
		}

		void deserialize(const Serializer* serializer) override
		{
			FlowNode::deserialize(serializer);
			String temp_name;
			serializer->read_string("var_name", temp_name);
			rename(temp_name);
			serializer->read_bool("var_global", var_ptr->global);
			serializer->read_byte_array("var_value", &var_ptr->get<T>(), sizeof(T));
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
			if (input_connected("write")) {
				var_ptr->store<T>(get_value<T>("write"));
			}
			call_next();
		}

		void rename(const StringView& new_name)
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
		StringMap<VarInfo>& storage() const
		{
			return parent->var_storage;
		}
	};
}

namespace titian {
	template<typename T>
	struct ConstructNode : Node
	{
		ConstructNode(NodeScript* parent, const StringView& title)
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
			else if constexpr (std::is_same_v<T, RGB>) {
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
		DeconstructNode(NodeScript* parent, const StringView& title)
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
			else if constexpr (std::is_same_v<T, RGB>) {
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

namespace titian {
	template<typename From, typename To>
	struct CastNode : Node
	{
		CastNode(NodeScript* parent, const StringView& title)
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
					return To(from);
				}
			});
		}
	};
}

namespace titian {
	template<typename From, typename To>
		requires std::is_base_of_v<From, To>
	struct IsTypeNode : FlowNode
	{
		IsTypeNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, false, ne::NodeStyle::orange())
		{
			addIN<void*>("ptr");
			addOUT<FlowNode*>("is")->behaviour([this]() { return this; });
			addOUT<FlowNode*>("not")->behaviour([this]() { return this; });
		}

		void call() override
		{
			From* ptr = (From*) get_value<void*>("ptr");
			if (dynamic_cast<To*>(ptr)) {
				call_next("is");
			}
			else {
				call_next("not");
			}
		}
	};
}

namespace titian {
	template<typename T>
		requires has_less_operator<T>::value or has_equals_operator<T>::value or has_greater_operator<T>::value
	struct CompareNode : Node
	{
		CompareNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::yellow())
		{
			addIN<T>("left");
			addIN<T>("right");
			if constexpr (has_less_operator<T>::value) {
				addOUT<bool>("less")->behaviour([this]
				{
					return get_value<T>("left") < get_value<T>("right");
				});
			}
			if constexpr (has_equals_operator<T>::value) {
				addOUT<bool>("equal")->behaviour([this]
				{
					return get_value<T>("left") == get_value<T>("right");
				});
			}
			if constexpr (has_greater_operator<T>::value) {
				addOUT<bool>("greater")->behaviour([this]
				{
					return get_value<T>("left") > get_value<T>("right");
				});
			}
		}
	};

	template<typename T>
	struct ContainsNode : Node
	{
		ContainsNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::yellow())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<bool>("contains")->behaviour([this]
			{
				const auto& left = get_value<T>("left");
				const auto& right = get_value<T>("right");
				return left.find(right) != -1;
			});
		}
	};
}

namespace titian {
	struct LogicNotNode : Node
	{
		LogicNotNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::pink())
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
		LogicAndNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::pink())
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
		LogicOrNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::pink())
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

namespace titian {
	template<typename T>
	struct OperatorNode : Node
	{
		OperatorNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::blue())
		{
			addIN<T>("left");
			addIN<T>("right");
			addOUT<T>("out")->behaviour([this] { return this->compute(); });
		}

		virtual T compute() = 0;
	};

	template<typename T>
	struct OperatorPlusNode : OperatorNode<T>
	{
		OperatorPlusNode(NodeScript* parent, const StringView& title)
			: OperatorNode<T>(parent, title)
		{}

		T compute() override
		{
			return this->get_value<T>("left") + this->get_value<T>("right");
		}
	};

	template<typename T>
	struct OperatorMinusNode : OperatorNode<T>
	{
		OperatorMinusNode(NodeScript* parent, const StringView& title)
			: OperatorNode<T>(parent, title)
		{}

		T compute() override
		{
			return this->get_value<T>("left") - this->get_value<T>("right");
		}
	};

	template<typename T>
	struct OperatorTimesNode : OperatorNode<T>
	{
		OperatorTimesNode(NodeScript* parent, const StringView& title)
			: OperatorNode<T>(parent, title)
		{}

		T compute() override
		{
			return this->get_value<T>("left") * this->get_value<T>("right");
		}
	};

	template<typename T>
	struct OperatorDivideNode : OperatorNode<T>
	{
		OperatorDivideNode(NodeScript* parent, const StringView& title)
			: OperatorNode<T>(parent, title)
		{}

		T compute() override
		{
			return this->get_value<T>("left") / this->get_value<T>("right");
		}
	};

	template<typename T>
	struct OperatorPowerNode : OperatorNode<T>
	{
		OperatorPowerNode(NodeScript* parent, const StringView& title)
			: OperatorNode<T>(parent, title)
		{}

		T compute() override
		{
			const T left = this->get_value<T>("left");
			const T right = this->get_value<T>("right");
			return T(::pow(double(left), double(right)));
		}
	};

	template<typename T>
	struct OperatorModuloNode : OperatorNode<T>
	{
		OperatorModuloNode(NodeScript* parent, const StringView& title)
			: OperatorNode<T>(parent, title)
		{}

		T compute() override
		{
			const T left = this->get_value<T>("left");
			const T right = this->get_value<T>("right");
			if constexpr (std::is_same_v<T, int32_t>) {
				return left % right;
			}
			else if constexpr (std::is_same_v<T, float>) {
				return T(::fmod(left, right));
			}
			else {
				static_assert(false, "Unsupported helper mod type");
			}
		}
	};

	template<typename T>
	struct OperatorMinNode : OperatorNode<T>
	{
		OperatorMinNode(NodeScript* parent, const StringView& title)
			: OperatorNode<T>(parent, title)
		{}

		T compute() override
		{
			const T left = this->get_value<T>("left");
			const T right = this->get_value<T>("right");
			return kl::min(left, right);
		}
	};

	template<typename T>
	struct OperatorMaxNode : OperatorNode<T>
	{
		OperatorMaxNode(NodeScript* parent, const StringView& title)
			: OperatorNode<T>(parent, title)
		{}

		T compute() override
		{
			const T left = this->get_value<T>("left");
			const T right = this->get_value<T>("right");
			return kl::max(left, right);
		}
	};
}

namespace titian {
	struct IfNode : FlowNode
	{
		IfNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, false, ne::NodeStyle::red())
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
		WhileNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::red())
		{
			addIN<bool>("value");
			addOUT<FlowNode*>("call")->behaviour([this] { return this; });
		}

		void call() override
		{
			while (true) {
				bool value = get_value<bool>("value");
				if (value) {
					call_next("call");
				}
				else {
					break;
				}
			}
			call_next();
		}
	};

	struct ForNode : FlowNode
	{
		ForNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::red())
		{
			addIN<int32_t>("from_incl");
			addIN<int32_t>("to_excl");
			addOUT<FlowNode*>("call")->behaviour([this] { return this; });
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
				call_next("call");
			}
			call_next();
		}
	};
}

namespace titian {
	struct FunctionNode : FlowNode
	{
		FunctionNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::crimson())
		{}
	};

	struct PrintNode : FunctionNode
	{
		PrintNode(NodeScript* parent, const StringView& title)
			: FunctionNode(parent, title)
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

namespace titian {
	template<typename T>
	struct MathNode : Node
	{
		MathNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::magenta())
		{
			this->addIN<T>("in");
			this->addOUT<T>("out")->behaviour([this] { return this->compute(); });
		}

		virtual T compute() = 0;
	};

	template<typename T>
	struct AbsNode : MathNode<T>
	{
		AbsNode(NodeScript* parent, const StringView& title)
			: MathNode<T>(parent, title)
		{}

		T compute() override
		{
			const T in = this->get_value<T>("in");
			return kl::abs(in);
		}
	};

	template<typename T>
	struct SqrtNode : MathNode<T>
	{
		SqrtNode(NodeScript* parent, const StringView& title)
			: MathNode<T>(parent, title)
		{}

		T compute() override
		{
			const T in = this->get_value<T>("in");
			return T(::sqrt(in));
		}
	};

	template<typename T>
	struct LogNode : MathNode<T>
	{
		LogNode(NodeScript* parent, const StringView& title)
			: MathNode<T>(parent, title)
		{}

		T compute() override
		{
			const T in = this->get_value<T>("in");
			return T(::log(in));
		}
	};

	template<typename T>
	struct TrigNode : MathNode<T>
	{
		TrigNode(NodeScript* parent, const StringView& title)
			: MathNode<T>(parent, title)
		{
			this->addIN<bool>("inverse");
			this->addIN<bool>("degrees");
		}

	protected:
		bool is_inverse()
		{
			return this->get_value<bool>("inverse");
		}

		bool is_degrees()
		{
			return this->get_value<bool>("degrees");
		}
	};

	template<typename T>
	struct SinNode : TrigNode<T>
	{
		SinNode(NodeScript* parent, const StringView& title)
			: TrigNode<T>(parent, title)
		{}

		T compute() override
		{
			const T in = this->get_value<T>("in");
			if (this->is_inverse()) {
				return (this->is_degrees() ? kl::asin_d<T> : kl::asin<T>)(in);
			}
			else {
				return (this->is_degrees() ? kl::sin_d<T> : kl::sin<T>)(in);
			}
		}
	};

	template<typename T>
	struct CosNode : TrigNode<T>
	{
		CosNode(NodeScript* parent, const StringView& title)
			: TrigNode<T>(parent, title)
		{}

		T compute() override
		{
			const T in = this->get_value<T>("in");
			if (this->is_inverse()) {
				return (this->is_degrees() ? kl::acos_d<T> : kl::acos<T>)(in);
			}
			else {
				return (this->is_degrees() ? kl::cos_d<T> : kl::cos<T>)(in);
			}
		}
	};

	template<typename T>
	struct TanNode : TrigNode<T>
	{
		TanNode(NodeScript* parent, const StringView& title)
			: TrigNode<T>(parent, title)
		{}

		T compute() override
		{
			const T in = this->get_value<T>("in");
			if (this->is_inverse()) {
				return (this->is_degrees() ? kl::atan_d<T> : kl::atan<T>)(in);
			}
			else {
				return (this->is_degrees() ? kl::tan_d<T> : kl::tan<T>)(in);
			}
		}
	};
}

namespace titian {
	struct GetSceneNode : Node
	{
		GetSceneNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::purple())
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

			addOUT<Float3>("gravity")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.gravity();
			});

			addOUT<String>("main_camera_name")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.main_camera_name;
			});
			addOUT<String>("main_ambient_light_name")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.main_ambient_light_name;
			});
			addOUT<String>("main_directional_light_name")->behaviour([this]()
			{
				Scene& scene = *Layers::get<GameLayer>()->scene;
				return scene.main_directional_light_name;
			});
		}
	};

	struct GetMeshNode : Node
	{
		GetMeshNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::purple())
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
		GetAnimationNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<int32_t>("animation_type")->behaviour([this]()
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
		GetTextureNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<Int2>("resolution")->behaviour([this]()
			{
				if (Texture* ptr = get_casted_value<void*, Texture*>("ptr")) {
					return ptr->resolution();
				}
				return Int2{};
			});
			addOUT<bool>("is_cube")->behaviour([this]()
			{
				if (Texture* ptr = get_casted_value<void*, Texture*>("ptr")) {
					return ptr->is_cube();
				}
				return false;
			});
		}
	};

	struct GetMaterialNode : Node
	{
		GetMaterialNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<float>("texture_blend")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->texture_blend;
				}
				return float{};
			});
			addOUT<float>("reflectivity_factor")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->reflectivity_factor;
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
			addOUT<Float4>("color")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->color;
				}
				return Float4{};
			});
			addOUT<String>("color_texture_name")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->color_texture_name;
				}
				return String{};
			});
			addOUT<String>("normal_texture_name")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->normal_texture_name;
				}
				return String{};
			});
			addOUT<String>("roughness_texture_name")->behaviour([this]()
			{
				if (Material* ptr = get_casted_value<void*, Material*>("ptr")) {
					return ptr->roughness_texture_name;
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
		GetShaderNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<int32_t>("shader_type")->behaviour([this]()
			{
				if (Shader* ptr = get_casted_value<void*, Shader*>("ptr")) {
					return (int32_t) ptr->shader_type;
				}
				return int32_t{};
			});
		}
	};

	struct GetEntityNode : Node
	{
		GetEntityNode(NodeScript* parent, const StringView& title)
			: Node(parent, title, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addOUT<bool>("casts_shadows")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->casts_shadows;
				}
				return bool{};
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
			addOUT<bool>("dynamic")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->dynamic();
				}
				return bool{};
			});
			addOUT<bool>("gravity")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->gravity();
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
			addOUT<float>("angular_damping")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->angular_damping();
				}
				return float{};
			});
			addOUT<Float3>("scale")->behaviour([this]()
			{
				if (Entity* ptr = get_casted_value<void*, Entity*>("ptr")) {
					return ptr->scale();
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
		}
	};
}

namespace titian {
	struct SetSceneNode : FlowNode
	{
		Mesh* mesh_ptr = nullptr;
		Animation* animation_ptr = nullptr;
		Texture* texture_ptr = nullptr;
		Material* material_ptr = nullptr;
		Shader* shader_ptr = nullptr;
		Entity* entity_ptr = nullptr;

		SetSceneNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::purple())
		{
			addIN<String>("mesh_name");
			addOUT<void*>("new mesh")->behaviour([this]() { return mesh_ptr; });

			addIN<String>("animation_name");
			addOUT<void*>("new animation")->behaviour([this]() { return animation_ptr; });

			addIN<String>("texture_name");
			addOUT<void*>("new texture")->behaviour([this]() { return texture_ptr; });

			addIN<String>("material_name");
			addOUT<void*>("new material")->behaviour([this]() { return material_ptr; });

			addIN<String>("shader_name");
			addOUT<void*>("new shader")->behaviour([this]() { return shader_ptr; });

			addIN<String>("entity_name");
			addOUT<void*>("new entity")->behaviour([this]() { return entity_ptr; });

			addIN<Float3>("gravity");
			addIN<String>("main_camera_name");
			addIN<String>("main_ambient_light_name");
			addIN<String>("main_directional_light_name");
		}

		void call() override
		{
			Scene* scene = scene = &Layers::get<GameLayer>()->scene;
			if (input_connected("mesh_name")) {
				mesh_ptr = scene->helper_new_mesh(get_value<String>("mesh_name"));
			}
			if (input_connected("animation_name")) {
				animation_ptr = scene->helper_new_animation(get_value<String>("animation_name"));
			}
			if (input_connected("texture_name")) {
				texture_ptr = scene->helper_new_texture(get_value<String>("texture_name"));
			}
			if (input_connected("material_name")) {
				material_ptr = scene->helper_new_material(get_value<String>("material_name"));
			}
			if (input_connected("shader_name")) {
				shader_ptr = scene->helper_new_shader(get_value<String>("shader_name"));
			}
			if (input_connected("entity_name")) {
				entity_ptr = scene->helper_new_entity(get_value<String>("entity_name"));
			}
			if (input_connected("gravity")) {
				scene->set_gravity(get_value<Float3>("gravity"));
			}
			if (input_connected("main_camera_name")) {
				scene->main_camera_name = get_value<String>("main_camera_name");
			}
			if (input_connected("main_ambient_light_name")) {
				scene->main_ambient_light_name = get_value<String>("main_ambient_light_name");
			}
			if (input_connected("main_directional_light_name")) {
				scene->main_directional_light_name = get_value<String>("main_directional_light_name");
			}
			call_next();
		}
	};

	struct SetMeshNode : FlowNode
	{
		SetMeshNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addIN<int32_t>("topology");
			addIN<bool>("wireframe");
		}

		void call() override
		{
			Mesh* ptr = get_casted_value<void*, Mesh*>("ptr");
			if (!ptr) {
				return;
			}

			if (input_connected("topology")) {
				ptr->topology = get_value<int32_t>("topology");
			}
			if (input_connected("wireframe")) {
				ptr->render_wireframe = get_value<bool>("wireframe");
			}
			call_next();
		}
	};

	struct SetAnimationNode : FlowNode
	{
		SetAnimationNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addIN<int32_t>("animation_type");
			addIN<float>("ticks per second");
			addIN<float>("duration ticks");
		}

		void call() override
		{
			Animation* ptr = get_casted_value<void*, Animation*>("ptr");
			if (!ptr) {
				return;
			}

			if (input_connected("animation_type")) {
				ptr->animation_type = get_value<int32_t>("animation_type");
			}
			if (input_connected("ticks per second")) {
				ptr->ticks_per_second = get_value<float>("ticks per second");
			}
			if (input_connected("duration ticks")) {
				ptr->duration_in_ticks = get_value<float>("duration ticks");
			}
			call_next();
		}
	};

	struct SetTextureNode : FlowNode
	{
		SetTextureNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addIN<Int2>("size");
		}

		void call() override
		{
			Texture* ptr = get_casted_value<void*, Texture*>("ptr");
			if (!ptr) {
				return;
			}

			if (input_connected("size")) {
				ptr->data_buffer.resize(get_value<Int2>("size"));
				if (ptr->is_cube()) {
					ptr->reload_as_cube();
				}
				else {
					ptr->reload_as_2D();
				}
			}
			call_next();
		}
	};

	struct SetMaterialNode : FlowNode
	{
		SetMaterialNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addIN<float>("texture_blend");
			addIN<float>("reflectivity_factor");
			addIN<float>("refraction_index");
			addIN<Float4>("color");
			addIN<String>("color_texture_name");
			addIN<String>("normal_texture_name");
			addIN<String>("roughness_texture_name");
			addIN<String>("shader_name");
		}

		void call() override
		{
			Material* ptr = get_casted_value<void*, Material*>("ptr");
			if (!ptr) {
				return;
			}

			if (input_connected("texture_blend")) {
				ptr->texture_blend = get_value<float>("texture_blend");
			}
			if (input_connected("reflectivity_factor")) {
				ptr->reflectivity_factor = get_value<float>("reflectivity_factor");
			}
			if (input_connected("refraction_index")) {
				ptr->refraction_index = get_value<float>("refraction_index");
			}
			if (input_connected("color")) {
				ptr->color = get_value<Float4>("color");
			}
			if (input_connected("color_texture_name")) {
				ptr->color_texture_name = get_value<String>("color_texture_name");
			}
			if (input_connected("normal_texture_name")) {
				ptr->normal_texture_name = get_value<String>("normal_texture_name");
			}
			if (input_connected("roughness_texture_name")) {
				ptr->roughness_texture_name = get_value<String>("roughness_texture_name");
			}
			if (input_connected("shader_name")) {
				ptr->shader_name = get_value<String>("shader_name");
			}
			call_next();
		}
	};

	struct SetShaderNode : FlowNode
	{
		SetShaderNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addIN<int32_t>("shader_type");
		}

		void call() override
		{
			Shader* ptr = get_casted_value<void*, Shader*>("ptr");
			if (!ptr) {
				return;
			}

			if (input_connected("shader_type")) {
				ptr->shader_type = (ShaderType) get_value<int32_t>("shader_type");
			}
			call_next();
		}
	};

	struct SetEntityNode : FlowNode
	{
		SetEntityNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::purple())
		{
			addIN<void*>("ptr");
			addIN<bool>("casts_shadows");
			addIN<String>("animation_name");
			addIN<String>("material_name");
			addIN<String>("collider_mesh_name");
			addIN<bool>("dynamic");
			addIN<bool>("gravity");
			addIN<float>("mass");
			addIN<float>("angular_damping"),
			addIN<Float3>("scale");
			addIN<Float3>("rotation");
			addIN<Float3>("position");
			addIN<Float3>("velocity");
			addIN<Float3>("angular");
		}

		void call() override
		{
			Entity* ptr = get_casted_value<void*, Entity*>("ptr");
			if (!ptr) {
				return;
			}

			if (input_connected("casts_shadows")) {
				ptr->casts_shadows = get_value<bool>("casts_shadows");
			}
			if (input_connected("animation_name")) {
				ptr->animation_name = get_value<String>("animation_name");
			}
			if (input_connected("material_name")) {
				ptr->material_name = get_value<String>("material_name");
			}
			if (input_connected("dynamic")) {
				ptr->set_dynamic(get_value<bool>("dynamic"));
			}
			if (input_connected("gravity")) {
				ptr->set_gravity(get_value<bool>("gravity"));
			}
			if (input_connected("mass")) {
				ptr->set_mass(get_value<float>("mass"));
			}
			if (input_connected("angular_damping")) {
				ptr->set_angular_damping(get_value<float>("angular_damping"));
			}
			if (input_connected("scale")) {
				ptr->set_scale(get_value<Float3>("scale"));
			}
			if (input_connected("rotation")) {
				ptr->set_rotation(get_value<Float3>("rotation"));
			}
			if (input_connected("position")) {
				ptr->set_position(get_value<Float3>("position"));
			}
			if (input_connected("velocity")) {
				ptr->set_velocity(get_value<Float3>("velocity"));
			}
			if (input_connected("angular")) {
				ptr->set_angular(get_value<Float3>("angular"));
			}
			call_next();
		}
	};
}

namespace titian {
	template<typename T>
	struct IteratorNode : FunctionNode
	{
		IteratorNode(NodeScript* parent, const StringView& title)
			: FunctionNode(parent, title)
		{
			addOUT<FlowNode*>("call")->behaviour([this] { return this; });
			addOUT<String>("name")->behaviour([this]()
			{
				return **reinterpret_cast<const String**>(user_data + 0);
			});
			addOUT<void*>("ptr")->behaviour([this]()
			{
				return *reinterpret_cast<void**>(user_data + 8);
			});
		}

		void call() final
		{
			iterate_collection([this](const String* name, void* ptr)
			{
				*reinterpret_cast<const String**>(user_data + 0) = name;
				*reinterpret_cast<void**>(user_data + 8) = ptr;
				call_next("call");
			});
			call_next();
		}

	protected:
		virtual void iterate_collection(const Function<void(const String*, void*)>& func) = 0;
	};

	struct IterateMeshesNode : IteratorNode<Mesh>
	{
		IterateMeshesNode(NodeScript* parent, const StringView& title)
			: IteratorNode<Mesh>(parent, title)
		{}

		void iterate_collection(const Function<void(const String*, void*)>& func) override
		{
			Layers::get<GameLayer>()->scene->helper_iterate_meshes([&](const String& name, Mesh* ptr)
			{
				func(&name, ptr);
			});
		}
	};

	struct IterateAnimationsNode : IteratorNode<Animation>
	{
		IterateAnimationsNode(NodeScript* parent, const StringView& title)
			: IteratorNode<Animation>(parent, title)
		{}

		void iterate_collection(const Function<void(const String*, void*)>& func) override
		{
			Layers::get<GameLayer>()->scene->helper_iterate_animations([&](const String& name, Animation* ptr)
			{
				func(&name, ptr);
			});
		}
	};

	struct IterateTexturesNode : IteratorNode<Texture>
	{
		IterateTexturesNode(NodeScript* parent, const StringView& title)
			: IteratorNode<Texture>(parent, title)
		{}

		void iterate_collection(const Function<void(const String*, void*)>& func) override
		{
			Layers::get<GameLayer>()->scene->helper_iterate_textures([&](const String& name, Texture* ptr)
			{
				func(&name, ptr);
			});
		}
	};

	struct IterateMaterialsNode : IteratorNode<Material>
	{
		IterateMaterialsNode(NodeScript* parent, const StringView& title)
			: IteratorNode<Material>(parent, title)
		{}

		void iterate_collection(const Function<void(const String*, void*)>& func) override
		{
			Layers::get<GameLayer>()->scene->helper_iterate_materials([&](const String& name, Material* ptr)
			{
				func(&name, ptr);
			});
		}
	};

	struct IterateShadersNode : IteratorNode<Shader>
	{
		IterateShadersNode(NodeScript* parent, const StringView& title)
			: IteratorNode<Shader>(parent, title)
		{}

		void iterate_collection(const Function<void(const String*, void*)>& func) override
		{
			Layers::get<GameLayer>()->scene->helper_iterate_shaders([&](const String& name, Shader* ptr)
			{
				func(&name, ptr);
			});
		}
	};

	struct IterateEntitiesNode : IteratorNode<Entity>
	{
		IterateEntitiesNode(NodeScript* parent, const StringView& title)
			: IteratorNode<Entity>(parent, title)
		{}

		void iterate_collection(const Function<void(const String*, void*)>& func) override
		{
			Layers::get<GameLayer>()->scene->helper_iterate_entities([&](const String& name, Entity* ptr)
			{
				func(&name, ptr);
			});
		}
	};
}

namespace titian {
	struct UISeparatorNode : FlowNode
	{
		UISeparatorNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::sunset())
		{}

		void call() override
		{
			ui_separator();
			call_next();
		}
	};

	struct UISameLineNode : FlowNode
	{
		UISameLineNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::sunset())
		{}

		void call() override
		{
			ui_same_line();
			call_next();
		}
	};

	struct UINextWidthNode : FlowNode
	{
		UINextWidthNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::sunset())
		{
			addIN<float>("width");
		}

		void call() override
		{
			float width = get_value<float>("width");
			ui_set_next_width(width);
			call_next();
		}
	};

	struct UICursorPositonNode : FlowNode
	{
		UICursorPositonNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::sunset())
		{
			addIN<Float2>("position");
			addOUT<Float2>("position")->behaviour([this]()
			{
				return ui_cursor_pos();
			});
		}

		void call() override
		{
			if (input_connected("position")) {
				Float2 pos = get_value<Float2>("position");
				ui_set_cursor_pos(pos);
			}
			call_next();
		}
	};

	struct UIWindowNode : FlowNode
	{
		UIWindowNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::sunset())
		{
			addIN<String>("name");
			addOUT<FlowNode*>("call")->behaviour([this]() { return this; });
		}

		void call() override
		{
			const String name = get_value<String>("name");
			ui_window(name, [this]()
			{
				call_next("call");
			});
			call_next();
		}
	};

	struct UIButtonNode : FlowNode
	{
		UIButtonNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::sunset())
		{
			addIN<String>("name");
			addOUT<FlowNode*>("call")->behaviour([this]() { return this; });
		}

		void call() override
		{
			const String name = get_value<String>("name");
			ui_button(name, [this]()
			{
				call_next("call");
			});
			call_next();
		}
	};

	template<typename T>
	struct UIValueNode : FlowNode
	{
		T value = {};
		
		UIValueNode(NodeScript* parent, const StringView& title)
			: FlowNode(parent, title, true, true, ne::NodeStyle::sunset())
		{
			addIN<String>("name");
			addIN<T>("value");
			addOUT<FlowNode*>("on_change")->behaviour([this]() { return this; });
			addOUT<T>("value")->behaviour([this]()
			{
				return value;
			});
		}

		void call() override
		{
			const auto on_set = [this](T new_val)
			{
				this->value = new_val;
				call_next("on_change");
			};
			if (input_connected("value")) {
				value = get_value<T>("value");
			}
			const String name = get_value<String>("name");
			if constexpr (std::is_same_v<T, bool>) {
				ui_bool(name, value, on_set);
			}
			else if constexpr (std::is_same_v<T, int32_t>) {
				ui_int(name, value, on_set);
			}
			else if constexpr (std::is_same_v<T, Int2>) {
				ui_int2(name, value, on_set);
			}
			else if constexpr (std::is_same_v<T, float>) {
				ui_float(name, value, on_set);
			}
			else if constexpr (std::is_same_v<T, Float2>) {
				ui_float2(name, value, on_set);
			}
			else if constexpr (std::is_same_v<T, Float3>) {
				ui_float3(name, value, on_set);
			}
			else if constexpr (std::is_same_v<T, Float4>) {
				ui_float4(name, value, on_set);
			}
			else if constexpr (std::is_same_v<T, RGB>) {
				ui_color(name, value, on_set);
			}
			else if constexpr (std::is_same_v<T, String>) {
				ui_string(name, value, on_set);
			}
			else {
				static_assert(false, "Not supported UIValueNode type");
			}
			call_next();
		}
	};
}
