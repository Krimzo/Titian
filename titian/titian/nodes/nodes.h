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
		ANIMATION,
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
		String title;
		PinType type;
		Node* node;

		Pin(const String& title, PinType type, Node* node);
	};

	class Node : public Unique
	{
	public:
		String title;
		Float2 screen_position;

		Optional<Pin> flow_input;
		Optional<Pin> flow_output;

		Vector<Pin> input_pins;
		Vector<Pin> output_pins;

		Function<void()> function;

		Node();

		void call();
	};
}
