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

// pin types
namespace titian {
	template<typename T>
	std::shared_ptr<ne::PinStyle> get_pin_style()
	{
		if constexpr (std::is_same_v<T, FlowNode*>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(255, 255, 255, 255), 0, 5.0f, 6.0f, 5.5f, 1.0f);
		}
		else if constexpr (std::is_same_v<T, bool>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(190, 90, 90, 255), 3, 5.0f, 6.0f, 5.5f, 1.0f);
		}
		else if constexpr (std::is_same_v<T, int32_t>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(75, 155, 215, 255), 4, 5.0f, 6.0f, 5.5f, 1.0f);
		}
		else if constexpr (std::is_same_v<T, float>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(75, 200, 175, 255), 5, 5.0f, 6.0f, 5.5f, 1.0f);
		}
		else if constexpr (std::is_same_v<T, String>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(215, 155, 135, 255), 6, 5.0f, 6.0f, 5.5f, 1.0f);
		}
		else if constexpr (std::is_same_v<T, void*>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(155, 85, 215, 255), 7, 5.0f, 6.0f, 5.5f, 1.0f);
		}
		else {
			static_assert(false, "Unkown pin style type");
		}
	}
}

// node types
namespace titian {
	struct Node : ne::BaseNode, Serializable
	{
		byte user_data[16] = {};

		inline Node(const String& title, const std::shared_ptr<ne::NodeStyle>& style)
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
		const T& get_value(const StringView& in_pin)
		{
			auto pin = reinterpret_cast<ne::InPin<T>*>(this->inPin(in_pin.data()));
			return pin->val();
		}
	};
}

namespace titian {
	template<typename T>
	struct LiteralNode : Node
	{
		T value = {};

		inline LiteralNode(const String& title)
			: Node(title, ne::NodeStyle::green())
		{
			addOUT<T>("result", get_pin_style<T>())->behaviour([this] { return this->value; });
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
			else {
				static_assert(false, "Unknown draw literal node type");
			}
		}
	};
}

namespace titian {
	template<typename From, typename To>
	struct CastNode : Node
	{
		inline CastNode(const String& title)
			: Node(title, ne::NodeStyle::orange())
		{
			addIN<From>("from", get_pin_style<From>());
			addOUT<To>("to", get_pin_style<To>())->behaviour([this]
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

namespace titian {
	template<typename T>
	struct CompareNode : Node
	{
		inline CompareNode(const String& title)
			: Node(title, ne::NodeStyle::yellow())
		{
			addIN<T>("left", get_pin_style<T>());
			addIN<T>("right", get_pin_style<T>());
			addOUT<bool>("less", get_pin_style<bool>())->behaviour([this]
			{
				return get_value<T>("left") < get_value<T>("right");
			});
			addOUT<bool>("equal", get_pin_style<bool>())->behaviour([this]
			{
				return get_value<T>("left") == get_value<T>("right");
			});
			addOUT<bool>("greater", get_pin_style<bool>())->behaviour([this]
			{
				return get_value<T>("left") > get_value<T>("right");
			});
		}
	};
}

namespace titian {
	struct LogicNotNode : Node
	{
		inline LogicNotNode()
			: Node("Not", ne::NodeStyle::pink())
		{
			addIN<bool>("in", get_pin_style<bool>());
			addOUT<bool>("out", get_pin_style<bool>())->behaviour([this]
			{
				return !get_value<bool>("in");
			});
		}
	};

	struct LogicAndNode : Node
	{
		inline LogicAndNode()
			: Node("And", ne::NodeStyle::pink())
		{
			addIN<bool>("left", get_pin_style<bool>());
			addIN<bool>("right", get_pin_style<bool>());
			addOUT<bool>("out", get_pin_style<bool>())->behaviour([this]
			{
				return get_value<bool>("left") && get_value<bool>("right");
			});
		}
	};

	struct LogicOrNode : Node
	{
		inline LogicOrNode()
			: Node("Or", ne::NodeStyle::pink())
		{
			addIN<bool>("left", get_pin_style<bool>());
			addIN<bool>("right", get_pin_style<bool>());
			addOUT<bool>("out", get_pin_style<bool>())->behaviour([this]
			{
				return get_value<bool>("left") || get_value<bool>("right");
			});
		}
	};
}

namespace titian {
	template<typename T>
	struct OperatorPlusNode : Node
	{
		inline OperatorPlusNode()
			: Node("Plus", ne::NodeStyle::blue())
		{
			addIN<T>("left", get_pin_style<T>());
			addIN<T>("right", get_pin_style<T>());
			addOUT<T>("out", get_pin_style<T>())->behaviour([this]
			{
				return get_value<T>("left") + get_value<T>("right");
			});
		}
	};

	template<typename T>
	struct OperatorMinusNode : Node
	{
		inline OperatorMinusNode()
			: Node("Minus", ne::NodeStyle::blue())
		{
			addIN<T>("left", get_pin_style<T>());
			addIN<T>("right", get_pin_style<T>());
			addOUT<T>("out", get_pin_style<T>())->behaviour([this]
			{
				return get_value<T>("left") - get_value<T>("right");
			});
		}
	};

	template<typename T>
	struct OperatorTimesNode : Node
	{
		inline OperatorTimesNode()
			: Node("Times", ne::NodeStyle::blue())
		{
			addIN<T>("left", get_pin_style<T>());
			addIN<T>("right", get_pin_style<T>());
			addOUT<T>("out", get_pin_style<T>())->behaviour([this]
			{
				return get_value<T>("left") * get_value<T>("right");
			});
		}
	};

	template<typename T>
	struct OperatorDivideNode : Node
	{
		inline OperatorDivideNode()
			: Node("Divide", ne::NodeStyle::blue())
		{
			addIN<T>("left", get_pin_style<T>());
			addIN<T>("right", get_pin_style<T>());
			addOUT<T>("out", get_pin_style<T>())->behaviour([this]
			{
				return get_value<T>("left") / get_value<T>("right");
			});
		}
	};

	template<typename T>
	struct OperatorPowerNode : Node
	{
		inline OperatorPowerNode()
			: Node("Power", ne::NodeStyle::blue())
		{
			addIN<T>("left", get_pin_style<T>());
			addIN<T>("right", get_pin_style<T>());
			addOUT<T>("out", get_pin_style<T>())->behaviour([this]
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
		inline OperatorModuloNode()
			: Node("Modulo", ne::NodeStyle::blue())
		{
			addIN<T>("left", get_pin_style<T>());
			addIN<T>("right", get_pin_style<T>());
			addOUT<T>("out", get_pin_style<T>())->behaviour([this]
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

namespace titian {
	struct FlowNode : Node
	{
		bool has_input = false;
		bool has_output = false;

		inline FlowNode(const String& title, bool has_input, bool has_output)
			: Node(title, ne::NodeStyle::red()), has_input(has_input), has_output(has_output)
		{
			if (has_input) {
				addIN<FlowNode*>("in_flow", get_pin_style<FlowNode*>());
			}
			if (has_output) {
				addOUT<FlowNode*>("out_flow", get_pin_style<FlowNode*>())->behaviour([this] { return this; });
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
				addIN<FlowNode*>("in_flow", get_pin_style<FlowNode*>());
			}
			if (!has_output && saved_has_ouput) {
				has_output = true;
				addOUT<FlowNode*>("out_flow", get_pin_style<FlowNode*>())->behaviour([this] { return this; });
			}
		}

		virtual void call()
		{
			call_next();
		}

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

namespace titian {
	struct IfNode : FlowNode
	{
		inline IfNode()
			: FlowNode("If", true, false)
		{
			addIN<bool>("value", get_pin_style<bool>());
			addOUT<FlowNode*>("if", get_pin_style<FlowNode*>())->behaviour([this] { return this; });
			addOUT<FlowNode*>("else", get_pin_style<FlowNode*>())->behaviour([this] { return this; });
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
}

namespace titian {
	struct WhileNode : FlowNode
	{
		inline WhileNode()
			: FlowNode("While", true, true)
		{
			addIN<bool>("value", get_pin_style<bool>());
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
}

namespace titian {
	struct ForNode : FlowNode
	{
		inline ForNode()
			: FlowNode("For", true, true)
		{
			addIN<int32_t>("from_incl", get_pin_style<int32_t>());
			addIN<int32_t>("to_excl", get_pin_style<int32_t>());
			addOUT<int32_t>("i", get_pin_style<int32_t>())->behaviour([this]()
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

namespace titian {
	struct PrintNode : FlowNode
	{
		inline PrintNode()
			: FlowNode("Print", true, true)
		{
			addIN<String>("value", get_pin_style<String>());
		}

		void call() override
		{
			auto& value = get_value<String>("value");
			Logger::log(value);
			call_next();
		}
	};
}
