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
			return std::make_shared<ne::PinStyle>(IM_COL32(255, 255, 255, 255), 0, 4.f, 4.67f, 3.7f, 1.f);
		}
		else if constexpr (std::is_same_v<T, bool>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(190, 90, 90, 255), 3, 4.f, 4.67f, 3.7f, 1.f);
		}
		else if constexpr (std::is_same_v<T, int32_t>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(75, 200, 175, 255), 4, 4.f, 4.67f, 3.7f, 1.f);
		}
		else if constexpr (std::is_same_v<T, float>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(75, 155, 215, 255), 5, 4.f, 4.67f, 3.7f, 1.f);
		}
		else if constexpr (std::is_same_v<T, String>) {
			return std::make_shared<ne::PinStyle>(IM_COL32(215, 155, 135, 255), 6, 4.f, 4.67f, 3.7f, 1.f);
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
		int64_t user_data = 0;

		inline Node(const StringView& title, const std::shared_ptr<ne::NodeStyle>& style)
		{
			setTitle(title.data());
			setStyle(style);
		}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			serializer->write_string("node_type", typeid(self).name()); // must be read from outside

			serializer->write_byte_array("user_data", &user_data, sizeof(user_data));
			serializer->write_string("title", getName());
			serializer->write_float_array("position", (const float*) &getPos(), 2);

			auto& style = *getStyle();
			serializer->write_byte_array("style", &style, sizeof(style));
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			serializer->read_byte_array("user_data", &user_data, sizeof(user_data));
			
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

		inline LiteralNode(const StringView& title)
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
		inline CastNode(const StringView& title)
			: Node(title, ne::NodeStyle::cyan())
		{
			addIN<From>("from", ne::ConnectionFilter::SameType(), get_pin_style<From>());
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
	struct FlowNode : Node
	{
		bool has_input = false;
		bool has_output = false;

		inline FlowNode(const StringView& title, bool has_input, bool has_output)
			: Node(title, ne::NodeStyle::red()), has_input(has_input), has_output(has_output)
		{
			if (has_input) {
				addIN<FlowNode*>("in_flow", ne::ConnectionFilter::SameType(), get_pin_style<FlowNode*>());
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
				addIN<FlowNode*>("in_flow", ne::ConnectionFilter::SameType(), get_pin_style<FlowNode*>());
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
			addIN<bool>("value", ne::ConnectionFilter::SameType(), get_pin_style<bool>());
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
			addIN<bool>("value", ne::ConnectionFilter::SameType(), get_pin_style<bool>());
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
			addIN<int32_t>("from_incl", ne::ConnectionFilter::SameType(), get_pin_style<int32_t>());
			addIN<int32_t>("to_excl", ne::ConnectionFilter::SameType(), get_pin_style<int32_t>());
			addOUT<int32_t>("i", get_pin_style<int32_t>())->behaviour([this]()
			{
				return static_cast<int32_t>(this->user_data);
			});
		}

		void call() override
		{
			auto& from = get_value<int32_t>("from_incl");
			auto& to = get_value<int32_t>("to_excl");
			for (int32_t i = from; i < to; i++) {
				user_data = i;
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
			addIN<String>("value", ne::ConnectionFilter::SameType(), get_pin_style<String>());
		}

		void call() override
		{
			auto& value = get_value<String>("value");
			Logger::log(value);
			call_next();
		}
	};
}
