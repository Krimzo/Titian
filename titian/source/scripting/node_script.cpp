#include "titian.h"


namespace titian {
	static const int _ignored = [&]
	{
		using namespace ne;
		register_pin_style<FlowNode*>(255, 255, 255, 3);
		register_pin_style<void*>(190, 85, 200, 4);
		register_pin_style<bool>(190, 90, 90, 4);
		register_pin_style<int32_t>(75, 155, 215, 4);
		register_pin_style<Int2>(75, 135, 215, 5);
		register_pin_style<float>(75, 200, 175, 4);
		register_pin_style<Float2>(75, 200, 155, 5);
		register_pin_style<Float3>(75, 200, 135, 6);
		register_pin_style<Float4>(75, 200, 115, 7);
		register_pin_style<Complex>(115, 70, 190, 4);
		register_pin_style<Quaternion>(100, 75, 205, 5);
		register_pin_style<Color>(185, 175, 70, 4);
		register_pin_style<String>(215, 155, 135, 4);
		return 0;
	}();
}

namespace titian {
	template<typename T>
	static constexpr Pair<String, Function<std::shared_ptr<Node>(NodeScript*)>> serial_generator_helper()
	{
		return { typeid(T).name(), [=](NodeScript* script) { return std::shared_ptr<T>(new T(script, {})); } };
	}

	static const Vector<Pair<String, Function<std::shared_ptr<Node>(NodeScript*)>>> serial_node_generators
	{
		// base nodes
		serial_generator_helper<Node>(),
		serial_generator_helper<FlowNode>(),

		// pointer
		serial_generator_helper<VariableNode<void*>>(),
		serial_generator_helper<CastNode<void*, bool>>(),
		serial_generator_helper<CastNode<void*, String>>(),
		serial_generator_helper<CompareNode<void*>>(),

		// bool
		serial_generator_helper<LiteralNode<bool>>(),
		serial_generator_helper<VariableNode<bool>>(),
		serial_generator_helper<CastNode<bool, int32_t>>(),
		serial_generator_helper<CastNode<bool, float>>(),
		serial_generator_helper<CastNode<bool, String>>(),
		serial_generator_helper<CompareNode<bool>>(),

		// int
		serial_generator_helper<LiteralNode<int32_t>>(),
		serial_generator_helper<VariableNode<int32_t>>(),
		serial_generator_helper<CastNode<int32_t, bool>>(),
		serial_generator_helper<CastNode<int32_t, float>>(),
		serial_generator_helper<CastNode<int32_t, String>>(),
		serial_generator_helper<CompareNode<int32_t>>(),
		serial_generator_helper<OperatorPlusNode<int32_t>>(),
		serial_generator_helper<OperatorMinusNode<int32_t>>(),
		serial_generator_helper<OperatorTimesNode<int32_t>>(),
		serial_generator_helper<OperatorDivideNode<int32_t>>(),
		serial_generator_helper<OperatorPowerNode<int32_t>>(),
		serial_generator_helper<OperatorModuloNode<int32_t>>(),
		serial_generator_helper<OperatorMinNode<int32_t>>(),
		serial_generator_helper<OperatorMaxNode<int32_t>>(),
		serial_generator_helper<AbsNode<int32_t>>(),

		// int2
		serial_generator_helper<LiteralNode<Int2>>(),
		serial_generator_helper<VariableNode<Int2>>(),
		serial_generator_helper<ConstructNode<Int2>>(),
		serial_generator_helper<DeconstructNode<Int2>>(),
		serial_generator_helper<CastNode<Int2, Float2>>(),
		serial_generator_helper<CastNode<Int2, String>>(),
		serial_generator_helper<CompareNode<Int2>>(),
		serial_generator_helper<OperatorPlusNode<Int2>>(),
		serial_generator_helper<OperatorMinusNode<Int2>>(),
		serial_generator_helper<OperatorTimesNode<Int2>>(),
		serial_generator_helper<OperatorDivideNode<Int2>>(),
		serial_generator_helper<OperatorMinNode<Int2>>(),
		serial_generator_helper<OperatorMaxNode<Int2>>(),
		serial_generator_helper<AbsNode<Int2>>(),

		// float
		serial_generator_helper<LiteralNode<float>>(),
		serial_generator_helper<VariableNode<float>>(),
		serial_generator_helper<CastNode<float, bool>>(),
		serial_generator_helper<CastNode<float, int32_t>>(),
		serial_generator_helper<CastNode<float, String>>(),
		serial_generator_helper<CompareNode<float>>(),
		serial_generator_helper<OperatorPlusNode<float>>(),
		serial_generator_helper<OperatorMinusNode<float>>(),
		serial_generator_helper<OperatorTimesNode<float>>(),
		serial_generator_helper<OperatorDivideNode<float>>(),
		serial_generator_helper<OperatorPowerNode<float>>(),
		serial_generator_helper<OperatorModuloNode<float>>(),
		serial_generator_helper<OperatorMinNode<float>>(),
		serial_generator_helper<OperatorMaxNode<float>>(),
		serial_generator_helper<AbsNode<float>>(),

		// float2
		serial_generator_helper<LiteralNode<Float2>>(),
		serial_generator_helper<VariableNode<Float2>>(),
		serial_generator_helper<ConstructNode<Float2>>(),
		serial_generator_helper<DeconstructNode<Float2>>(),
		serial_generator_helper<CastNode<Float2, Int2>>(),
		serial_generator_helper<CastNode<Float2, Complex>>(),
		serial_generator_helper<CastNode<Float2, String>>(),
		serial_generator_helper<CompareNode<Float2>>(),
		serial_generator_helper<OperatorPlusNode<Float2>>(),
		serial_generator_helper<OperatorMinusNode<Float2>>(),
		serial_generator_helper<OperatorTimesNode<Float2>>(),
		serial_generator_helper<OperatorDivideNode<Float2>>(),
		serial_generator_helper<OperatorMinNode<Float2>>(),
		serial_generator_helper<OperatorMaxNode<Float2>>(),
		serial_generator_helper<AbsNode<Float2>>(),

		// float3
		serial_generator_helper<LiteralNode<Float3>>(),
		serial_generator_helper<VariableNode<Float3>>(),
		serial_generator_helper<ConstructNode<Float3>>(),
		serial_generator_helper<DeconstructNode<Float3>>(),
		serial_generator_helper<CastNode<Float3, Quaternion>>(),
		serial_generator_helper<CastNode<Float3, Color>>(),
		serial_generator_helper<CastNode<Float3, String>>(),
		serial_generator_helper<CompareNode<Float3>>(),
		serial_generator_helper<OperatorPlusNode<Float3>>(),
		serial_generator_helper<OperatorMinusNode<Float3>>(),
		serial_generator_helper<OperatorTimesNode<Float3>>(),
		serial_generator_helper<OperatorDivideNode<Float3>>(),
		serial_generator_helper<OperatorMinNode<Float3>>(),
		serial_generator_helper<OperatorMaxNode<Float3>>(),
		serial_generator_helper<AbsNode<Float3>>(),

		// float4
		serial_generator_helper<LiteralNode<Float4>>(),
		serial_generator_helper<VariableNode<Float4>>(),
		serial_generator_helper<ConstructNode<Float4>>(),
		serial_generator_helper<DeconstructNode<Float4>>(),
		serial_generator_helper<CastNode<Float4, Quaternion>>(),
		serial_generator_helper<CastNode<Float4, Color>>(),
		serial_generator_helper<CastNode<Float4, String>>(),
		serial_generator_helper<CompareNode<Float4>>(),
		serial_generator_helper<OperatorPlusNode<Float4>>(),
		serial_generator_helper<OperatorMinusNode<Float4>>(),
		serial_generator_helper<OperatorTimesNode<Float4>>(),
		serial_generator_helper<OperatorDivideNode<Float4>>(),
		serial_generator_helper<OperatorMinNode<Float4>>(),
		serial_generator_helper<OperatorMaxNode<Float4>>(),
		serial_generator_helper<AbsNode<Float4>>(),

		// complex
		serial_generator_helper<LiteralNode<Complex>>(),
		serial_generator_helper<VariableNode<Complex>>(),
		serial_generator_helper<ConstructNode<Complex>>(),
		serial_generator_helper<DeconstructNode<Complex>>(),
		serial_generator_helper<CastNode<Complex, Float2>>(),
		serial_generator_helper<CastNode<Complex, String>>(),
		serial_generator_helper<CompareNode<Complex>>(),
		serial_generator_helper<OperatorPlusNode<Complex>>(),
		serial_generator_helper<OperatorMinusNode<Complex>>(),
		serial_generator_helper<OperatorTimesNode<Complex>>(),
		serial_generator_helper<OperatorMinNode<Complex>>(),
		serial_generator_helper<OperatorMaxNode<Complex>>(),
		serial_generator_helper<AbsNode<Complex>>(),

		// quaternion
		serial_generator_helper<LiteralNode<Quaternion>>(),
		serial_generator_helper<VariableNode<Quaternion>>(),
		serial_generator_helper<ConstructNode<Quaternion>>(),
		serial_generator_helper<DeconstructNode<Quaternion>>(),
		serial_generator_helper<CastNode<Quaternion, Float3>>(),
		serial_generator_helper<CastNode<Quaternion, Float4>>(),
		serial_generator_helper<CastNode<Quaternion, String>>(),
		serial_generator_helper<CompareNode<Quaternion>>(),
		serial_generator_helper<OperatorPlusNode<Quaternion>>(),
		serial_generator_helper<OperatorMinusNode<Quaternion>>(),
		serial_generator_helper<OperatorTimesNode<Quaternion>>(),
		serial_generator_helper<OperatorMinNode<Quaternion>>(),
		serial_generator_helper<OperatorMaxNode<Quaternion>>(),
		serial_generator_helper<AbsNode<Quaternion>>(),

		// color
		serial_generator_helper<LiteralNode<Color>>(),
		serial_generator_helper<VariableNode<Color>>(),
		serial_generator_helper<ConstructNode<Color>>(),
		serial_generator_helper<DeconstructNode<Color>>(),
		serial_generator_helper<CastNode<Color, Float3>>(),
		serial_generator_helper<CastNode<Color, Float4>>(),
		serial_generator_helper<CompareNode<Color>>(),

		// string
		serial_generator_helper<LiteralNode<String>>(),
		serial_generator_helper<VariableNode<String>>(),
		serial_generator_helper<CastNode<String, bool>>(),
		serial_generator_helper<CastNode<String, int32_t>>(),
		serial_generator_helper<CastNode<String, float>>(),
		serial_generator_helper<CompareNode<String>>(),
		serial_generator_helper<OperatorPlusNode<String>>(),
		serial_generator_helper<ContainsNode<String>>(),

		// scene
		serial_generator_helper<GetSceneNode>(),
		serial_generator_helper<GetMeshNode>(),
		serial_generator_helper<GetAnimationNode>(),
		serial_generator_helper<GetTextureNode>(),
		serial_generator_helper<GetMaterialNode>(),
		serial_generator_helper<GetShaderNode>(),
		serial_generator_helper<GetEntityNode>(),
		serial_generator_helper<SetSceneNode>(),
		serial_generator_helper<SetMeshNode>(),
		serial_generator_helper<SetAnimationNode>(),
		serial_generator_helper<SetTextureNode>(),
		serial_generator_helper<SetMaterialNode>(),
		serial_generator_helper<SetShaderNode>(),
		serial_generator_helper<SetEntityNode>(),
		serial_generator_helper<IterateMeshesNode>(),
		serial_generator_helper<IterateAnimationsNode>(),
		serial_generator_helper<IterateTexturesNode>(),
		serial_generator_helper<IterateMaterialsNode>(),
		serial_generator_helper<IterateShadersNode>(),
		serial_generator_helper<IterateEntitiesNode>(),

		// logic
		serial_generator_helper<LogicNotNode>(),
		serial_generator_helper<LogicAndNode>(),
		serial_generator_helper<LogicOrNode>(),

		// flow
		serial_generator_helper<IfNode>(),
		serial_generator_helper<WhileNode>(),
		serial_generator_helper<ForNode>(),

		// is
		serial_generator_helper<IsTypeNode<Entity, Camera>>(),
		serial_generator_helper<IsTypeNode<Entity, AmbientLight>>(),
		serial_generator_helper<IsTypeNode<Entity, PointLight>>(),
		serial_generator_helper<IsTypeNode<Entity, DirectionalLight>>(),

		// math
		serial_generator_helper<SqrtNode<float>>(),
		serial_generator_helper<SinNode<float>>(),
		serial_generator_helper<CosNode<float>>(),
		serial_generator_helper<TanNode<float>>(),

		// debug
		serial_generator_helper<PrintNode>(),
	};
}

namespace titian {
	template<typename T, typename... Args>
	static constexpr Pair<String, Function<std::shared_ptr<Node>(NodeScript*)>> ui_generator_helper(const char* name)
	{
		return { name, [=](NodeScript* script) { return std::shared_ptr<T>(new T(script, name)); } };
	}

	static const Vector<Pair<String, Vector<Pair<String, Function<std::shared_ptr<Node>(NodeScript*)>>>>> ui_node_generators = {
		{ "Pointer",
		{
		ui_generator_helper<VariableNode<void*>>("Variable Pointer"),
		ui_generator_helper<CastNode<void*, bool>>("Pointer -> Bool"),
		ui_generator_helper<CastNode<void*, String>>("Pointer -> String"),
		ui_generator_helper<CompareNode<void*>>("Compare Pointer"),
		},
		},
		
		{ "Bool",
		{
		ui_generator_helper<LiteralNode<bool>>("Literal Bool"),
		ui_generator_helper<VariableNode<bool>>("Variable Bool"),
		ui_generator_helper<CastNode<bool, int32_t>>("Bool -> Int"),
		ui_generator_helper<CastNode<bool, float>>("Bool -> Float"),
		ui_generator_helper<CastNode<bool, String>>("Bool -> String"),
		ui_generator_helper<CompareNode<bool>>("Compare Bool"),
		},
		},

		{ "Int",
		{
		ui_generator_helper<LiteralNode<int32_t>>("Literal Int"),
		ui_generator_helper<VariableNode<int32_t>>("Variable Int"),
		ui_generator_helper<CastNode<int32_t, bool>>("Int -> Bool"),
		ui_generator_helper<CastNode<int32_t, float>>("Int -> Float"),
		ui_generator_helper<CastNode<int32_t, String>>("Int -> String"),
		ui_generator_helper<CompareNode<int32_t>>("Compare Int"),
		ui_generator_helper<OperatorPlusNode<int32_t>>("Plus Int"),
		ui_generator_helper<OperatorMinusNode<int32_t>>("Minus Int"),
		ui_generator_helper<OperatorTimesNode<int32_t>>("Times Int"),
		ui_generator_helper<OperatorDivideNode<int32_t>>("Divide Int"),
		ui_generator_helper<OperatorPowerNode<int32_t>>("Power Int"),
		ui_generator_helper<OperatorModuloNode<int32_t>>("Modulo Int"),
		ui_generator_helper<OperatorMinNode<int32_t>>("Min Int"),
		ui_generator_helper<OperatorMaxNode<int32_t>>("Max Int"),
		ui_generator_helper<AbsNode<int32_t>>("Abs Int"),
		},
		},

		{ "Int2",
		{
		ui_generator_helper<LiteralNode<Int2>>("Literal Int2"),
		ui_generator_helper<VariableNode<Int2>>("Variable Int2"),
		ui_generator_helper<ConstructNode<Int2>>("Construct Int2"),
		ui_generator_helper<DeconstructNode<Int2>>("Deconstruct Int2"),
		ui_generator_helper<CastNode<Int2, Float2>>("Int2 -> Float2"),
		ui_generator_helper<CastNode<Int2, String>>("Int2 -> String"),
		ui_generator_helper<CompareNode<Int2>>("Compare Int2"),
		ui_generator_helper<OperatorPlusNode<Int2>>("Plus Int2"),
		ui_generator_helper<OperatorMinusNode<Int2>>("Minus Int2"),
		ui_generator_helper<OperatorTimesNode<Int2>>("Times Int2"),
		ui_generator_helper<OperatorDivideNode<Int2>>("Divide Int2"),
		ui_generator_helper<OperatorMinNode<Int2>>("Min Int2"),
		ui_generator_helper<OperatorMaxNode<Int2>>("Max Int2"),
		ui_generator_helper<AbsNode<Int2>>("Abs Int2"),
		},
		},

		{ "Float",
		{
		ui_generator_helper<LiteralNode<float>>("Literal Float"),
		ui_generator_helper<VariableNode<float>>("Variable Float"),
		ui_generator_helper<CastNode<float, bool>>("Float -> Bool"),
		ui_generator_helper<CastNode<float, int32_t>>("Float -> Int"),
		ui_generator_helper<CastNode<float, String>>("Float -> String"),
		ui_generator_helper<CompareNode<float>>("Compare Float"),
		ui_generator_helper<OperatorPlusNode<float>>("Plus Float"),
		ui_generator_helper<OperatorMinusNode<float>>("Minus Float"),
		ui_generator_helper<OperatorTimesNode<float>>("Times Float"),
		ui_generator_helper<OperatorDivideNode<float>>("Divide Float"),
		ui_generator_helper<OperatorPowerNode<float>>("Power Float"),
		ui_generator_helper<OperatorModuloNode<float>>("Modulo Float"),
		ui_generator_helper<OperatorMinNode<float>>("Min Float"),
		ui_generator_helper<OperatorMaxNode<float>>("Max Float"),
		ui_generator_helper<AbsNode<float>>("Abs Float"),
		},
		},

		{ "Float2",
		{
		ui_generator_helper<LiteralNode<Float2>>("Literal Float2"),
		ui_generator_helper<VariableNode<Float2>>("Variable Float2"),
		ui_generator_helper<ConstructNode<Float2>>("Construct Float2"),
		ui_generator_helper<DeconstructNode<Float2>>("Deconstruct Float2"),
		ui_generator_helper<CastNode<Float2, Int2>>("Float2 -> Int2"),
		ui_generator_helper<CastNode<Float2, Complex>>("Float2 -> Complex"),
		ui_generator_helper<CastNode<Float2, String>>("Float2 -> String"),
		ui_generator_helper<CompareNode<Float2>>("Compare Float2"),
		ui_generator_helper<OperatorPlusNode<Float2>>("Plus Float2"),
		ui_generator_helper<OperatorMinusNode<Float2>>("Minus Float2"),
		ui_generator_helper<OperatorTimesNode<Float2>>("Times Float2"),
		ui_generator_helper<OperatorDivideNode<Float2>>("Divide Float2"),
		ui_generator_helper<OperatorMinNode<Float2>>("Min Float2"),
		ui_generator_helper<OperatorMaxNode<Float2>>("Max Float2"),
		ui_generator_helper<AbsNode<Float2>>("Abs Float2"),
		},
		},

		{ "Float3",
		{
		ui_generator_helper<LiteralNode<Float3>>("Literal Float3"),
		ui_generator_helper<VariableNode<Float3>>("Variable Float3"),
		ui_generator_helper<ConstructNode<Float3>>("Construct Float3"),
		ui_generator_helper<DeconstructNode<Float3>>("Deconstruct Float3"),
		ui_generator_helper<CastNode<Float3, Quaternion>>("Float3 -> Quaternion"),
		ui_generator_helper<CastNode<Float3, Color>>("Float3 -> Color"),
		ui_generator_helper<CastNode<Float3, String>>("Float3 -> String"),
		ui_generator_helper<CompareNode<Float3>>("Compare Float3"),
		ui_generator_helper<OperatorPlusNode<Float3>>("Plus Float3"),
		ui_generator_helper<OperatorMinusNode<Float3>>("Minus Float3"),
		ui_generator_helper<OperatorTimesNode<Float3>>("Times Float3"),
		ui_generator_helper<OperatorDivideNode<Float3>>("Divide Float3"),
		ui_generator_helper<OperatorMinNode<Float3>>("Min Float3"),
		ui_generator_helper<OperatorMaxNode<Float3>>("Max Float3"),
		ui_generator_helper<AbsNode<Float3>>("Abs Float3"),
		},
		},

		{ "Float4",
		{
		ui_generator_helper<LiteralNode<Float4>>("Literal Float4"),
		ui_generator_helper<VariableNode<Float4>>("Variable Float4"),
		ui_generator_helper<ConstructNode<Float4>>("Construct Float4"),
		ui_generator_helper<DeconstructNode<Float4>>("Deconstruct Float4"),
		ui_generator_helper<CastNode<Float4, Quaternion>>("Float4 -> Quaternion"),
		ui_generator_helper<CastNode<Float4, Color>>("Float4 -> Color"),
		ui_generator_helper<CastNode<Float4, String>>("Float4 -> String"),
		ui_generator_helper<CompareNode<Float4>>("Compare Float4"),
		ui_generator_helper<OperatorPlusNode<Float4>>("Plus Float4"),
		ui_generator_helper<OperatorMinusNode<Float4>>("Minus Float4"),
		ui_generator_helper<OperatorTimesNode<Float4>>("Times Float4"),
		ui_generator_helper<OperatorDivideNode<Float4>>("Divide Float4"),
		ui_generator_helper<OperatorMinNode<Float4>>("Min Float4"),
		ui_generator_helper<OperatorMaxNode<Float4>>("Max Float4"),
		ui_generator_helper<AbsNode<Float4>>("Abs Float4"),
		},
		},

		{ "Complex",
		{
		ui_generator_helper<LiteralNode<Complex>>("Literal Complex"),
		ui_generator_helper<VariableNode<Complex>>("Variable Complex"),
		ui_generator_helper<ConstructNode<Complex>>("Construct Complex"),
		ui_generator_helper<DeconstructNode<Complex>>("Deconstruct Complex"),
		ui_generator_helper<CastNode<Complex, Float2>>("Complex -> Float2"),
		ui_generator_helper<CastNode<Complex, String>>("Complex -> String"),
		ui_generator_helper<CompareNode<Complex>>("Compare Complex"),
		ui_generator_helper<OperatorPlusNode<Complex>>("Plus Complex"),
		ui_generator_helper<OperatorMinusNode<Complex>>("Minus Complex"),
		ui_generator_helper<OperatorTimesNode<Complex>>("Times Complex"),
		ui_generator_helper<OperatorMinNode<Complex>>("Min Complex"),
		ui_generator_helper<OperatorMaxNode<Complex>>("Max Complex"),
		ui_generator_helper<AbsNode<Complex>>("Abs Complex"),
		},
		},

		{ "Quaternion",
		{
		ui_generator_helper<LiteralNode<Quaternion>>("Literal Quaternion"),
		ui_generator_helper<VariableNode<Quaternion>>("Variable Quaternion"),
		ui_generator_helper<ConstructNode<Quaternion>>("Construct Quaternion"),
		ui_generator_helper<DeconstructNode<Quaternion>>("Deconstruct Quaternion"),
		ui_generator_helper<CastNode<Quaternion, Float3>>("Quaternion -> Float3"),
		ui_generator_helper<CastNode<Quaternion, Float4>>("Quaternion -> Float4"),
		ui_generator_helper<CastNode<Quaternion, String>>("Quaternion -> String"),
		ui_generator_helper<CompareNode<Quaternion>>("Compare Quaternion"),
		ui_generator_helper<OperatorPlusNode<Quaternion>>("Plus Quaternion"),
		ui_generator_helper<OperatorMinusNode<Quaternion>>("Minus Quaternion"),
		ui_generator_helper<OperatorTimesNode<Quaternion>>("Times Quaternion"),
		ui_generator_helper<OperatorMinNode<Quaternion>>("Min Quaternion"),
		ui_generator_helper<OperatorMaxNode<Quaternion>>("Max Quaternion"),
		ui_generator_helper<AbsNode<Quaternion>>("Abs Quaternion"),
		},
		},

		{ "Color",
		{
		ui_generator_helper<LiteralNode<Color>>("Literal Color"),
		ui_generator_helper<VariableNode<Color>>("Variable Color"),
		ui_generator_helper<ConstructNode<Color>>("Construct Color"),
		ui_generator_helper<DeconstructNode<Color>>("Deconstruct Color"),
		ui_generator_helper<CastNode<Color, Float3>>("Color -> Float3"),
		ui_generator_helper<CastNode<Color, Float4>>("Color -> Float4"),
		ui_generator_helper<CompareNode<Color>>("Compare Color"),
		},
		},

		{ "String",
		{
		ui_generator_helper<LiteralNode<String>>("Literal String"),
		ui_generator_helper<VariableNode<String>>("Variable String"),
		ui_generator_helper<CastNode<String, bool>>("String -> Bool"),
		ui_generator_helper<CastNode<String, int32_t>>("String -> Int"),
		ui_generator_helper<CastNode<String, float>>("String -> Float"),
		ui_generator_helper<CompareNode<String>>("Compare String"),
		ui_generator_helper<OperatorPlusNode<String>>("Plus String"),
		ui_generator_helper<ContainsNode<String>>("Contains String"),
		},
		},

		{ "Scene",
		{
		ui_generator_helper<GetSceneNode>("Get Scene"),
		ui_generator_helper<GetMeshNode>("Get Mesh"),
		ui_generator_helper<GetAnimationNode>("Get Animation"),
		ui_generator_helper<GetTextureNode>("Get Texture"),
		ui_generator_helper<GetMaterialNode>("Get Material"),
		ui_generator_helper<GetShaderNode>("Get Shader"),
		ui_generator_helper<GetEntityNode>("Get Entity"),
		ui_generator_helper<SetSceneNode>("Set Scene"),
		ui_generator_helper<SetMeshNode>("Set Mesh"),
		ui_generator_helper<SetAnimationNode>("Set Animation"),
		ui_generator_helper<SetTextureNode>("Set Texture"),
		ui_generator_helper<SetMaterialNode>("Set Material"),
		ui_generator_helper<SetShaderNode>("Set Shader"),
		ui_generator_helper<SetEntityNode>("Set Entity"),
		ui_generator_helper<IterateMeshesNode>("Iterate Meshes"),
		ui_generator_helper<IterateAnimationsNode>("Iterate Animations"),
		ui_generator_helper<IterateTexturesNode>("Iterate Textures"),
		ui_generator_helper<IterateMaterialsNode>("Iterate Materials"),
		ui_generator_helper<IterateShadersNode>("Iterate Shaders"),
		ui_generator_helper<IterateEntitiesNode>("Iterate Entities"),
		},
		},

		{ "Logic",
		{
		ui_generator_helper<LogicNotNode>("Not"),
		ui_generator_helper<LogicAndNode>("And"),
		ui_generator_helper<LogicOrNode>("Or"),
		},
		},

		{ "Flow",
		{
		ui_generator_helper<IfNode>("If"),
		ui_generator_helper<WhileNode>("While"),
		ui_generator_helper<ForNode>("For"),
		},
		},

		{ "Is",
		{
		ui_generator_helper<IsTypeNode<Entity, Camera>>("Is Camera"),
		ui_generator_helper<IsTypeNode<Entity, AmbientLight>>("Is Ambient Light"),
		ui_generator_helper<IsTypeNode<Entity, PointLight>>("Is Point Light"),
		ui_generator_helper<IsTypeNode<Entity, DirectionalLight>>("Is Directional Light"),
		},
		},

		{ "Math",
		{
		ui_generator_helper<SqrtNode<float>>("Sqrt"),
		ui_generator_helper<LogNode<float>>("Log"),
		ui_generator_helper<SinNode<float>>("Sin"),
		ui_generator_helper<CosNode<float>>("Cos"),
		ui_generator_helper<TanNode<float>>("Tan"),
		},
		},
		
		{ "Debug",
		{
		ui_generator_helper<PrintNode>("Print"),
		},
		},
	};
}

titian::NodeScript::NodeScript()
{
	kl::Timer* timer = &Layers::get<AppLayer>()->timer;

	auto temp_start_node = std::make_shared<FlowNode>(this, "On Start", false, true);
	auto temp_update_node = std::make_shared<FlowNode>(this, "On Update", false, true);
	auto temp_collision_node = std::make_shared<FlowNode>(this, "On Collision", false, true);
	auto temp_ui_node = std::make_shared<FlowNode>(this, "On UI", false, true);

	temp_start_node->setUID(1);
	temp_update_node->setUID(2);
	temp_collision_node->setUID(3);
	temp_ui_node->setUID(4);

	on_start_node = m_editor.insertNode({ 0.0f, -250.0f }, temp_start_node).get();
	on_update_node = m_editor.insertNode({ 0.0f, -83.33f }, temp_update_node).get();
	on_collision_node = m_editor.insertNode({ 0.0f, 83.33f }, temp_collision_node).get();
	on_ui_node = m_editor.insertNode({ 0.0f, 250.0f }, temp_ui_node).get();

	on_update_node->addOUT<float>("elapsed_time")
		->behaviour([timer]() { return timer->elapsed(); });
	on_update_node->addOUT<float>("delta_time")
		->behaviour([timer]() { return timer->delta(); });

	on_collision_node->addOUT<void*>("first_entity")
		->behaviour([this]() { return *reinterpret_cast<Entity**>(on_collision_node->user_data + 0); });
	on_collision_node->addOUT<void*>("second_entity")
		->behaviour([this]() { return *reinterpret_cast<Entity**>(on_collision_node->user_data + 8); });

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
		for (const auto& [tab_name, tab] : ui_node_generators) {
			if (!filter.empty() && tab_name.find(filter) == -1)
				continue;

			if (im::BeginMenu(tab_name.c_str())) {
				for (auto& [node_name, generator] : tab) {
					if (im::Selectable(node_name.c_str())) {
						m_editor.insertNode(m_editor.screen2grid(im::GetMousePos()), generator(this));
						im::CloseCurrentPopup();
					}
				}
				im::EndMenu();
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
				node = generator(this);
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
