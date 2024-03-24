#pragma once

#include "utility/id_util.h"


namespace titian {
	using PinKind = ax::NodeEditor::PinKind;

	enum class PinType
	{
		VOID = 0,
		FLOW,

		BOOL,
		INT,
		FLOAT,

		INT2,
		FLOAT2,
		FLOAT3,
		FLOAT4,

		FLOAT2X2,
		FLOAT3X3,
		FLOAT4X4,

		COMPLEX,
		QUATERNION,

		PLANE,
		SPHERE,
		RAY,

		VERTEX,
		TRIANGLE,

		MESH,
		TEXTURE,
		MATERIAL,

		ENTITY,
		SCENE,

		WINDOW,
		KEYBOARD,
		MOUSE,
	};
}

namespace titian {
	class Node;

	class Pin : public Unique
	{
	public:
		std::string title;
		PinType type;
		Node* node;

		inline Pin(const std::string& title, PinType type, Node* node)
			: title(title), type(type), node(node)
		{}
	};

	class Node : public Unique
	{
	public:
		std::string title;
		kl::Float2 screen_position;

		std::optional<Pin> flow_input;
		std::optional<Pin> flow_output;

		std::vector<Pin> input_pins;
		std::vector<Pin> output_pins;

		std::function<void()> function;

		inline Node()
		{}

		inline void call()
		{
			if (function) {
				function();
			}
			if (flow_output && flow_output.value().node) {
				flow_output.value().node->call();
			}
		}
	};
}
