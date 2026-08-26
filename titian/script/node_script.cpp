#include "titian.h"

namespace titian
{
static int _pin_init = [] {
    using namespace ne;
    register_pin_style<FlowNode*>(255, 255, 255, 3);
    register_pin_style<void*>(190, 85, 200, 4);
    register_pin_style<bool>(190, 90, 90, 4);
    register_pin_style<int32_t>(75, 155, 215, 4);
    register_pin_style<int2>(75, 135, 215, 5);
    register_pin_style<float>(75, 200, 175, 4);
    register_pin_style<float2>(75, 200, 155, 5);
    register_pin_style<float3>(75, 200, 135, 6);
    register_pin_style<float4>(75, 200, 115, 7);
    register_pin_style<cmplx>(115, 70, 190, 4);
    register_pin_style<quat>(100, 75, 205, 5);
    register_pin_style<RGB>(185, 175, 70, 4);
    register_pin_style<String>(215, 155, 135, 4);
    return 0;
}();
} // namespace titian

namespace titian
{
template <typename T> static constexpr Pair<String, Func<std::shared_ptr<Node>(NodeScript*)>> serial_generator_helper()
{
    return {typeid(T).name(), [=](NodeScript* script) { return std::shared_ptr<T>(new T(script, {})); }};
}

static Vector<Pair<String, Func<std::shared_ptr<Node>(NodeScript*)>>> serial_node_generators{
    serial_generator_helper<Node>(),
    serial_generator_helper<FlowNode>(),

    serial_generator_helper<VariableNode<void*>>(),
    serial_generator_helper<CastNode<void*, bool>>(),
    serial_generator_helper<CastNode<void*, String>>(),
    serial_generator_helper<CompareNode<void*>>(),

    serial_generator_helper<LiteralNode<bool>>(),
    serial_generator_helper<VariableNode<bool>>(),
    serial_generator_helper<RandomNode<bool>>(),
    serial_generator_helper<CastNode<bool, int32_t>>(),
    serial_generator_helper<CastNode<bool, float>>(),
    serial_generator_helper<CastNode<bool, String>>(),
    serial_generator_helper<CompareNode<bool>>(),

    serial_generator_helper<LiteralNode<int32_t>>(),
    serial_generator_helper<VariableNode<int32_t>>(),
    serial_generator_helper<RandomNode<int32_t>>(),
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

    serial_generator_helper<LiteralNode<int2>>(),
    serial_generator_helper<VariableNode<int2>>(),
    serial_generator_helper<RandomNode<int2>>(),
    serial_generator_helper<ConstructNode<int2>>(),
    serial_generator_helper<DeconstructNode<int2>>(),
    serial_generator_helper<CastNode<int2, float2>>(),
    serial_generator_helper<CastNode<int2, String>>(),
    serial_generator_helper<CompareNode<int2>>(),
    serial_generator_helper<OperatorPlusNode<int2>>(),
    serial_generator_helper<OperatorMinusNode<int2>>(),
    serial_generator_helper<OperatorTimesNode<int2>>(),
    serial_generator_helper<OperatorDivideNode<int2>>(),
    serial_generator_helper<OperatorMinNode<int2>>(),
    serial_generator_helper<OperatorMaxNode<int2>>(),
    serial_generator_helper<AbsNode<int2>>(),

    serial_generator_helper<LiteralNode<float>>(),
    serial_generator_helper<VariableNode<float>>(),
    serial_generator_helper<RandomNode<float>>(),
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

    serial_generator_helper<LiteralNode<float2>>(),
    serial_generator_helper<VariableNode<float2>>(),
    serial_generator_helper<RandomNode<float2>>(),
    serial_generator_helper<ConstructNode<float2>>(),
    serial_generator_helper<DeconstructNode<float2>>(),
    serial_generator_helper<CastNode<float2, int2>>(),
    serial_generator_helper<CastNode<float2, cmplx>>(),
    serial_generator_helper<CastNode<float2, String>>(),
    serial_generator_helper<CompareNode<float2>>(),
    serial_generator_helper<OperatorPlusNode<float2>>(),
    serial_generator_helper<OperatorMinusNode<float2>>(),
    serial_generator_helper<OperatorTimesNode<float2>>(),
    serial_generator_helper<OperatorDivideNode<float2>>(),
    serial_generator_helper<OperatorMinNode<float2>>(),
    serial_generator_helper<OperatorMaxNode<float2>>(),
    serial_generator_helper<AbsNode<float2>>(),

    serial_generator_helper<LiteralNode<float3>>(),
    serial_generator_helper<VariableNode<float3>>(),
    serial_generator_helper<RandomNode<float3>>(),
    serial_generator_helper<ConstructNode<float3>>(),
    serial_generator_helper<DeconstructNode<float3>>(),
    serial_generator_helper<CastNode<float3, quat>>(),
    serial_generator_helper<CastNode<float3, RGB>>(),
    serial_generator_helper<CastNode<float3, String>>(),
    serial_generator_helper<CompareNode<float3>>(),
    serial_generator_helper<OperatorPlusNode<float3>>(),
    serial_generator_helper<OperatorMinusNode<float3>>(),
    serial_generator_helper<OperatorTimesNode<float3>>(),
    serial_generator_helper<OperatorDivideNode<float3>>(),
    serial_generator_helper<OperatorMinNode<float3>>(),
    serial_generator_helper<OperatorMaxNode<float3>>(),
    serial_generator_helper<AbsNode<float3>>(),

    serial_generator_helper<LiteralNode<float4>>(),
    serial_generator_helper<VariableNode<float4>>(),
    serial_generator_helper<RandomNode<float4>>(),
    serial_generator_helper<ConstructNode<float4>>(),
    serial_generator_helper<DeconstructNode<float4>>(),
    serial_generator_helper<CastNode<float4, quat>>(),
    serial_generator_helper<CastNode<float4, RGB>>(),
    serial_generator_helper<CastNode<float4, String>>(),
    serial_generator_helper<CompareNode<float4>>(),
    serial_generator_helper<OperatorPlusNode<float4>>(),
    serial_generator_helper<OperatorMinusNode<float4>>(),
    serial_generator_helper<OperatorTimesNode<float4>>(),
    serial_generator_helper<OperatorDivideNode<float4>>(),
    serial_generator_helper<OperatorMinNode<float4>>(),
    serial_generator_helper<OperatorMaxNode<float4>>(),
    serial_generator_helper<AbsNode<float4>>(),

    serial_generator_helper<LiteralNode<cmplx>>(),
    serial_generator_helper<VariableNode<cmplx>>(),
    serial_generator_helper<RandomNode<cmplx>>(),
    serial_generator_helper<ConstructNode<cmplx>>(),
    serial_generator_helper<DeconstructNode<cmplx>>(),
    serial_generator_helper<CastNode<cmplx, float2>>(),
    serial_generator_helper<CastNode<cmplx, String>>(),
    serial_generator_helper<CompareNode<cmplx>>(),
    serial_generator_helper<OperatorPlusNode<cmplx>>(),
    serial_generator_helper<OperatorMinusNode<cmplx>>(),
    serial_generator_helper<OperatorTimesNode<cmplx>>(),
    serial_generator_helper<OperatorMinNode<cmplx>>(),
    serial_generator_helper<OperatorMaxNode<cmplx>>(),
    serial_generator_helper<AbsNode<cmplx>>(),

    serial_generator_helper<LiteralNode<quat>>(),
    serial_generator_helper<VariableNode<quat>>(),
    serial_generator_helper<RandomNode<quat>>(),
    serial_generator_helper<ConstructNode<quat>>(),
    serial_generator_helper<DeconstructNode<quat>>(),
    serial_generator_helper<CastNode<quat, float3>>(),
    serial_generator_helper<CastNode<quat, float4>>(),
    serial_generator_helper<CastNode<quat, String>>(),
    serial_generator_helper<CompareNode<quat>>(),
    serial_generator_helper<OperatorPlusNode<quat>>(),
    serial_generator_helper<OperatorMinusNode<quat>>(),
    serial_generator_helper<OperatorTimesNode<quat>>(),
    serial_generator_helper<OperatorMinNode<quat>>(),
    serial_generator_helper<OperatorMaxNode<quat>>(),
    serial_generator_helper<AbsNode<quat>>(),

    serial_generator_helper<LiteralNode<RGB>>(),
    serial_generator_helper<VariableNode<RGB>>(),
    serial_generator_helper<RandomNode<RGB>>(),
    serial_generator_helper<ConstructNode<RGB>>(),
    serial_generator_helper<DeconstructNode<RGB>>(),
    serial_generator_helper<CastNode<RGB, float3>>(),
    serial_generator_helper<CastNode<RGB, float4>>(),
    serial_generator_helper<CompareNode<RGB>>(),

    serial_generator_helper<LiteralNode<String>>(),
    serial_generator_helper<VariableNode<String>>(),
    serial_generator_helper<RandomNode<String>>(),
    serial_generator_helper<CastNode<String, bool>>(),
    serial_generator_helper<CastNode<String, int32_t>>(),
    serial_generator_helper<CastNode<String, float>>(),
    serial_generator_helper<CompareNode<String>>(),
    serial_generator_helper<OperatorPlusNode<String>>(),
    serial_generator_helper<ContainsNode<String>>(),

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

    serial_generator_helper<LogicNotNode>(),
    serial_generator_helper<LogicAndNode>(),
    serial_generator_helper<LogicOrNode>(),

    serial_generator_helper<IfNode>(),
    serial_generator_helper<WhileNode>(),
    serial_generator_helper<ForNode>(),

    serial_generator_helper<IsTypeNode<Entity, Camera>>(),
    serial_generator_helper<IsTypeNode<Entity, AmbientLight>>(),
    serial_generator_helper<IsTypeNode<Entity, DirectionalLight>>(),

    serial_generator_helper<SqrtNode<float>>(),
    serial_generator_helper<SinNode<float>>(),
    serial_generator_helper<CosNode<float>>(),
    serial_generator_helper<TanNode<float>>(),

    serial_generator_helper<UISeparatorNode>(),
    serial_generator_helper<UISameLineNode>(),
    serial_generator_helper<UINextWidthNode>(),
    serial_generator_helper<UIWindowNode>(),
    serial_generator_helper<UIButtonNode>(),
    serial_generator_helper<UIValueNode<bool>>(),
    serial_generator_helper<UIValueNode<int32_t>>(),
    serial_generator_helper<UIValueNode<int2>>(),
    serial_generator_helper<UIValueNode<float>>(),
    serial_generator_helper<UIValueNode<float2>>(),
    serial_generator_helper<UIValueNode<float3>>(),
    serial_generator_helper<UIValueNode<float4>>(),
    serial_generator_helper<UIValueNode<RGB>>(),
    serial_generator_helper<UIValueNode<String>>(),

    serial_generator_helper<PrintNode>(),
};
} // namespace titian

namespace titian
{
template <typename T, typename... Args>
static constexpr Pair<String, Func<std::shared_ptr<Node>(NodeScript*)>> ui_generator_helper(str name)
{
    return {name, [=](NodeScript* script) { return std::shared_ptr<T>(new T(script, name)); }};
}

static Vector<Pair<String, Vector<Pair<String, Func<std::shared_ptr<Node>(NodeScript*)>>>>> ui_node_generators = {
    {
        "Pointer",
        {
            ui_generator_helper<VariableNode<void*>>("Variable Pointer"),
            ui_generator_helper<CastNode<void*, bool>>("Pointer -> bool"),
            ui_generator_helper<CastNode<void*, String>>("Pointer -> String"),
            ui_generator_helper<CompareNode<void*>>("Compare Pointer"),
        },
    },

    {
        "bool",
        {
            ui_generator_helper<LiteralNode<bool>>("Literal bool"),
            ui_generator_helper<VariableNode<bool>>("Variable bool"),
            ui_generator_helper<RandomNode<bool>>("Random bool"),
            ui_generator_helper<CastNode<bool, int32_t>>("bool -> int"),
            ui_generator_helper<CastNode<bool, float>>("bool -> float"),
            ui_generator_helper<CastNode<bool, String>>("bool -> String"),
            ui_generator_helper<CompareNode<bool>>("Compare bool"),
        },
    },

    {
        "int",
        {
            ui_generator_helper<LiteralNode<int32_t>>("Literal int"),
            ui_generator_helper<VariableNode<int32_t>>("Variable int"),
            ui_generator_helper<RandomNode<int32_t>>("Random int"),
            ui_generator_helper<CastNode<int32_t, bool>>("int -> bool"),
            ui_generator_helper<CastNode<int32_t, float>>("int -> float"),
            ui_generator_helper<CastNode<int32_t, String>>("int -> String"),
            ui_generator_helper<CompareNode<int32_t>>("Compare int"),
            ui_generator_helper<OperatorPlusNode<int32_t>>("Plus int"),
            ui_generator_helper<OperatorMinusNode<int32_t>>("Minus int"),
            ui_generator_helper<OperatorTimesNode<int32_t>>("Times int"),
            ui_generator_helper<OperatorDivideNode<int32_t>>("Divide int"),
            ui_generator_helper<OperatorPowerNode<int32_t>>("Power int"),
            ui_generator_helper<OperatorModuloNode<int32_t>>("Modulo int"),
            ui_generator_helper<OperatorMinNode<int32_t>>("Min int"),
            ui_generator_helper<OperatorMaxNode<int32_t>>("Max int"),
            ui_generator_helper<AbsNode<int32_t>>("Abs int"),
        },
    },

    {
        "int2",
        {
            ui_generator_helper<LiteralNode<int2>>("Literal int2"),
            ui_generator_helper<VariableNode<int2>>("Variable int2"),
            ui_generator_helper<RandomNode<int2>>("Random int2"),
            ui_generator_helper<ConstructNode<int2>>("Construct int2"),
            ui_generator_helper<DeconstructNode<int2>>("Deconstruct int2"),
            ui_generator_helper<CastNode<int2, float2>>("int2 -> float2"),
            ui_generator_helper<CastNode<int2, String>>("int2 -> String"),
            ui_generator_helper<CompareNode<int2>>("Compare int2"),
            ui_generator_helper<OperatorPlusNode<int2>>("Plus int2"),
            ui_generator_helper<OperatorMinusNode<int2>>("Minus int2"),
            ui_generator_helper<OperatorTimesNode<int2>>("Times int2"),
            ui_generator_helper<OperatorDivideNode<int2>>("Divide int2"),
            ui_generator_helper<OperatorMinNode<int2>>("Min int2"),
            ui_generator_helper<OperatorMaxNode<int2>>("Max int2"),
            ui_generator_helper<AbsNode<int2>>("Abs int2"),
        },
    },

    {
        "float",
        {
            ui_generator_helper<LiteralNode<float>>("Literal float"),
            ui_generator_helper<VariableNode<float>>("Variable float"),
            ui_generator_helper<RandomNode<float>>("Random float"),
            ui_generator_helper<CastNode<float, bool>>("float -> bool"),
            ui_generator_helper<CastNode<float, int32_t>>("float -> int"),
            ui_generator_helper<CastNode<float, String>>("float -> String"),
            ui_generator_helper<CompareNode<float>>("Compare float"),
            ui_generator_helper<OperatorPlusNode<float>>("Plus float"),
            ui_generator_helper<OperatorMinusNode<float>>("Minus float"),
            ui_generator_helper<OperatorTimesNode<float>>("Times float"),
            ui_generator_helper<OperatorDivideNode<float>>("Divide float"),
            ui_generator_helper<OperatorPowerNode<float>>("Power float"),
            ui_generator_helper<OperatorModuloNode<float>>("Modulo float"),
            ui_generator_helper<OperatorMinNode<float>>("Min float"),
            ui_generator_helper<OperatorMaxNode<float>>("Max float"),
            ui_generator_helper<AbsNode<float>>("Abs float"),
        },
    },

    {
        "float2",
        {
            ui_generator_helper<LiteralNode<float2>>("Literal float2"),
            ui_generator_helper<VariableNode<float2>>("Variable float2"),
            ui_generator_helper<RandomNode<float2>>("Random float2"),
            ui_generator_helper<ConstructNode<float2>>("Construct float2"),
            ui_generator_helper<DeconstructNode<float2>>("Deconstruct float2"),
            ui_generator_helper<CastNode<float2, int2>>("float2 -> int2"),
            ui_generator_helper<CastNode<float2, cmplx>>("float2 -> cmplx"),
            ui_generator_helper<CastNode<float2, String>>("float2 -> String"),
            ui_generator_helper<CompareNode<float2>>("Compare float2"),
            ui_generator_helper<OperatorPlusNode<float2>>("Plus float2"),
            ui_generator_helper<OperatorMinusNode<float2>>("Minus float2"),
            ui_generator_helper<OperatorTimesNode<float2>>("Times float2"),
            ui_generator_helper<OperatorDivideNode<float2>>("Divide float2"),
            ui_generator_helper<OperatorMinNode<float2>>("Min float2"),
            ui_generator_helper<OperatorMaxNode<float2>>("Max float2"),
            ui_generator_helper<AbsNode<float2>>("Abs float2"),
        },
    },

    {
        "float3",
        {
            ui_generator_helper<LiteralNode<float3>>("Literal float3"),
            ui_generator_helper<VariableNode<float3>>("Variable float3"),
            ui_generator_helper<RandomNode<float3>>("Random float3"),
            ui_generator_helper<ConstructNode<float3>>("Construct float3"),
            ui_generator_helper<DeconstructNode<float3>>("Deconstruct float3"),
            ui_generator_helper<CastNode<float3, quat>>("float3 -> quat"),
            ui_generator_helper<CastNode<float3, RGB>>("float3 -> Color"),
            ui_generator_helper<CastNode<float3, String>>("float3 -> String"),
            ui_generator_helper<CompareNode<float3>>("Compare float3"),
            ui_generator_helper<OperatorPlusNode<float3>>("Plus float3"),
            ui_generator_helper<OperatorMinusNode<float3>>("Minus float3"),
            ui_generator_helper<OperatorTimesNode<float3>>("Times float3"),
            ui_generator_helper<OperatorDivideNode<float3>>("Divide float3"),
            ui_generator_helper<OperatorMinNode<float3>>("Min float3"),
            ui_generator_helper<OperatorMaxNode<float3>>("Max float3"),
            ui_generator_helper<AbsNode<float3>>("Abs float3"),
        },
    },

    {
        "float4",
        {
            ui_generator_helper<LiteralNode<float4>>("Literal float4"),
            ui_generator_helper<VariableNode<float4>>("Variable float4"),
            ui_generator_helper<RandomNode<float4>>("Random float4"),
            ui_generator_helper<ConstructNode<float4>>("Construct float4"),
            ui_generator_helper<DeconstructNode<float4>>("Deconstruct float4"),
            ui_generator_helper<CastNode<float4, quat>>("float4 -> quat"),
            ui_generator_helper<CastNode<float4, RGB>>("float4 -> Color"),
            ui_generator_helper<CastNode<float4, String>>("float4 -> String"),
            ui_generator_helper<CompareNode<float4>>("Compare float4"),
            ui_generator_helper<OperatorPlusNode<float4>>("Plus float4"),
            ui_generator_helper<OperatorMinusNode<float4>>("Minus float4"),
            ui_generator_helper<OperatorTimesNode<float4>>("Times float4"),
            ui_generator_helper<OperatorDivideNode<float4>>("Divide float4"),
            ui_generator_helper<OperatorMinNode<float4>>("Min float4"),
            ui_generator_helper<OperatorMaxNode<float4>>("Max float4"),
            ui_generator_helper<AbsNode<float4>>("Abs float4"),
        },
    },

    {
        "cmplx",
        {
            ui_generator_helper<LiteralNode<cmplx>>("Literal cmplx"),
            ui_generator_helper<VariableNode<cmplx>>("Variable cmplx"),
            ui_generator_helper<RandomNode<cmplx>>("Random cmplx"),
            ui_generator_helper<ConstructNode<cmplx>>("Construct cmplx"),
            ui_generator_helper<DeconstructNode<cmplx>>("Deconstruct cmplx"),
            ui_generator_helper<CastNode<cmplx, float2>>("cmplx -> float2"),
            ui_generator_helper<CastNode<cmplx, String>>("cmplx -> String"),
            ui_generator_helper<CompareNode<cmplx>>("Compare cmplx"),
            ui_generator_helper<OperatorPlusNode<cmplx>>("Plus cmplx"),
            ui_generator_helper<OperatorMinusNode<cmplx>>("Minus cmplx"),
            ui_generator_helper<OperatorTimesNode<cmplx>>("Times cmplx"),
            ui_generator_helper<OperatorMinNode<cmplx>>("Min cmplx"),
            ui_generator_helper<OperatorMaxNode<cmplx>>("Max cmplx"),
            ui_generator_helper<AbsNode<cmplx>>("Abs cmplx"),
        },
    },

    {
        "quat",
        {
            ui_generator_helper<LiteralNode<quat>>("Literal quat"),
            ui_generator_helper<VariableNode<quat>>("Variable quat"),
            ui_generator_helper<RandomNode<quat>>("Random quat"),
            ui_generator_helper<ConstructNode<quat>>("Construct quat"),
            ui_generator_helper<DeconstructNode<quat>>("Deconstruct quat"),
            ui_generator_helper<CastNode<quat, float3>>("quat -> float3"),
            ui_generator_helper<CastNode<quat, float4>>("quat -> float4"),
            ui_generator_helper<CastNode<quat, String>>("quat -> String"),
            ui_generator_helper<CompareNode<quat>>("Compare quat"),
            ui_generator_helper<OperatorPlusNode<quat>>("Plus quat"),
            ui_generator_helper<OperatorMinusNode<quat>>("Minus quat"),
            ui_generator_helper<OperatorTimesNode<quat>>("Times quat"),
            ui_generator_helper<OperatorMinNode<quat>>("Min quat"),
            ui_generator_helper<OperatorMaxNode<quat>>("Max quat"),
            ui_generator_helper<AbsNode<quat>>("Abs quat"),
        },
    },

    {
        "Color",
        {
            ui_generator_helper<LiteralNode<RGB>>("Literal Color"),
            ui_generator_helper<VariableNode<RGB>>("Variable Color"),
            ui_generator_helper<RandomNode<RGB>>("Random Color"),
            ui_generator_helper<ConstructNode<RGB>>("Construct Color"),
            ui_generator_helper<DeconstructNode<RGB>>("Deconstruct Color"),
            ui_generator_helper<CastNode<RGB, float3>>("Color -> float3"),
            ui_generator_helper<CastNode<RGB, float4>>("Color -> float4"),
            ui_generator_helper<CompareNode<RGB>>("Compare Color"),
        },
    },

    {
        "String",
        {
            ui_generator_helper<LiteralNode<String>>("Literal String"),
            ui_generator_helper<VariableNode<String>>("Variable String"),
            ui_generator_helper<RandomNode<String>>("Random String"),
            ui_generator_helper<CastNode<String, bool>>("String -> bool"),
            ui_generator_helper<CastNode<String, int32_t>>("String -> int"),
            ui_generator_helper<CastNode<String, float>>("String -> float"),
            ui_generator_helper<CompareNode<String>>("Compare String"),
            ui_generator_helper<OperatorPlusNode<String>>("Plus String"),
            ui_generator_helper<ContainsNode<String>>("Contains String"),
        },
    },

    {
        "Scene",
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

    {
        "Logic",
        {
            ui_generator_helper<LogicNotNode>("Not"),
            ui_generator_helper<LogicAndNode>("And"),
            ui_generator_helper<LogicOrNode>("Or"),
        },
    },

    {
        "Flow",
        {
            ui_generator_helper<IfNode>("If"),
            ui_generator_helper<WhileNode>("While"),
            ui_generator_helper<ForNode>("For"),
        },
    },

    {
        "Is",
        {
            ui_generator_helper<IsTypeNode<Entity, Camera>>("Is Camera"),
            ui_generator_helper<IsTypeNode<Entity, AmbientLight>>("Is Ambient Light"),
            ui_generator_helper<IsTypeNode<Entity, DirectionalLight>>("Is Directional Light"),
        },
    },

    {
        "Math",
        {
            ui_generator_helper<SqrtNode<float>>("Sqrt"),
            ui_generator_helper<LogNode<float>>("Log"),
            ui_generator_helper<SinNode<float>>("Sin"),
            ui_generator_helper<CosNode<float>>("Cos"),
            ui_generator_helper<TanNode<float>>("Tan"),
        },
    },

    {
        "UI",
        {
            ui_generator_helper<UISeparatorNode>("UI Separator"),
            ui_generator_helper<UISameLineNode>("UI Same Line"),
            ui_generator_helper<UINextWidthNode>("UI Next Width"),
            ui_generator_helper<UIWindowNode>("UI Window"),
            ui_generator_helper<UIButtonNode>("UI Button"),
            ui_generator_helper<UIValueNode<bool>>("UI Value bool"),
            ui_generator_helper<UIValueNode<int32_t>>("UI Value int"),
            ui_generator_helper<UIValueNode<int2>>("UI Value int2"),
            ui_generator_helper<UIValueNode<float>>("UI Value float"),
            ui_generator_helper<UIValueNode<float2>>("UI Value float2"),
            ui_generator_helper<UIValueNode<float3>>("UI Value float3"),
            ui_generator_helper<UIValueNode<float4>>("UI Value float4"),
            ui_generator_helper<UIValueNode<RGB>>("UI Value Color"),
            ui_generator_helper<UIValueNode<String>>("UI Value String"),
        },
    },

    {
        "Debug",
        {
            ui_generator_helper<PrintNode>("Print"),
        },
    },
};
} // namespace titian

titian::NodeScript::NodeScript()
{
    auto temp_start_node = std::make_shared<FlowNode>(this, "On Start", false, true);
    auto temp_update_node = std::make_shared<FlowNode>(this, "On Update", false, true);
    auto temp_collision_node = std::make_shared<FlowNode>(this, "On Collision", false, true);
    auto temp_ui_node = std::make_shared<FlowNode>(this, "On UI", false, true);

    temp_start_node->setUID(1);
    temp_update_node->setUID(2);
    temp_collision_node->setUID(3);
    temp_ui_node->setUID(4);

    on_start_node = m_editor.insertNode({0.0f, -250.0f}, temp_start_node).get();
    on_update_node = m_editor.insertNode({0.0f, -83.33f}, temp_update_node).get();
    on_collision_node = m_editor.insertNode({0.0f, 83.33f}, temp_collision_node).get();
    on_ui_node = m_editor.insertNode({0.0f, 250.0f}, temp_ui_node).get();

    on_update_node->addOUT<float>("elapsed_time")->behaviour([this]() { return AppLayer::get().timer.elapsed(); });
    on_update_node->addOUT<float>("delta_time")->behaviour([this]() { return AppLayer::get().timer.delta(); });

    on_collision_node->addOUT<void*>("attacker_entity")->behaviour([this]() {
        return *reinterpret_cast<Entity**>(on_collision_node->user_data + 0);
    });
    on_collision_node->addOUT<void*>("target_entity")->behaviour([this]() {
        return *reinterpret_cast<Entity**>(on_collision_node->user_data + 8);
    });

    m_editor.rightClickPopUpContent([this](ne::BaseNode* node) {
        if (!node)
        {
            new_node_popup();
            return;
        }
        if (node == on_start_node || node == on_update_node || node == on_collision_node || node == on_ui_node)
            return;
        if (im::Selectable("Destroy"))
        {
            node->destroy();
            im::CloseCurrentPopup();
        }
    });
    m_editor.setScroll({150.f, 450.f});
}

void titian::NodeScript::serialize(Serializer& serializer) const
{
    Script::serialize(serializer);

    serializer.write_float("editor_scale", m_editor.getScale());
    serializer.write_float_array("editor_scroll", (float*)&m_editor.getScroll(), 2);

    auto& nodes = m_editor.getNodes();
    serializer.write_int("node_count", (int32_t)nodes.size() - 4);

    int counter = 0;
    for (auto& [uid, node] : nodes)
    {
        auto node_ptr = dynamic_cast<Node*>(node.get());
        if (!node_ptr)
            ti_assert(false, "Each node type needs to be derived from Node!");

        if (node_ptr == on_start_node || node_ptr == on_update_node || node_ptr == on_collision_node ||
            node_ptr == on_ui_node)
            continue;

        String name = kl::format("__node_", counter);
        serializer.push_object(name);
        serializer.write_byte_array("uid", &uid, sizeof(uid));
        node_ptr->serialize(serializer);
        serializer.pop_object();
        counter += 1;
    }

    auto& links = m_editor.getLinks();
    serializer.write_int("link_count", (int32_t)links.size());

    counter = 0;
    for (auto& link : links)
    {
        auto link_ptr = link.lock();
        auto left_parent = link_ptr->left()->getParent()->getUID();
        auto right_parent = link_ptr->right()->getParent()->getUID();
        auto& left_name = link_ptr->left()->getName();
        auto& right_name = link_ptr->right()->getName();

        String name = kl::format("__link_", counter);
        serializer.push_object(name);
        serializer.write_byte_array("left_parent", &left_parent, sizeof(left_parent));
        serializer.write_byte_array("right_parent", &right_parent, sizeof(right_parent));
        serializer.write_string("left_name", left_name);
        serializer.write_string("right_name", right_name);
        serializer.pop_object();
        counter += 1;
    }
}

void titian::NodeScript::deserialize(Serializer const& serializer)
{
    Script::deserialize(serializer);

    float editor_scale = 0.0f;
    serializer.read_float("editor_scale", editor_scale);
    m_editor.setScale(editor_scale);

    ImVec2 editor_scroll = {};
    serializer.read_float_array("editor_scroll", (float*)&editor_scroll, 2);
    m_editor.setScroll(editor_scroll);

    int node_count = 0;
    serializer.read_int("node_count", node_count);

    for (int i = 0; i < node_count; i++)
    {
        String name = kl::format("__node_", i);
        serializer.load_object(name);

        ne::NodeUID uid{};
        serializer.read_byte_array("uid", &uid, sizeof(uid));

        String node_type;
        serializer.read_string("node_type", node_type);

        std::shared_ptr<Node> node;
        for (auto& [type, generator] : serial_node_generators)
        {
            if (node_type == type)
            {
                node = generator(this);
                break;
            }
        }
        ti_assert((bool)node, "Unknown node type: ", node_type);

        node->deserialize(serializer);
        node->setUID(uid);
        m_editor.insertNode(node);

        serializer.unload_object();
    }

    int link_count = 0;
    serializer.read_int("link_count", link_count);

    auto& nodes = m_editor.getNodes();
    for (int i = 0; i < link_count; i++)
    {
        String name = kl::format("__link_", i);
        serializer.load_object(name);

        ne::NodeUID left_parent{};
        ne::NodeUID right_parent{};
        serializer.read_byte_array("left_parent", &left_parent, sizeof(left_parent));
        serializer.read_byte_array("right_parent", &right_parent, sizeof(right_parent));

        String left_name;
        String right_name;
        serializer.read_string("left_name", left_name);
        serializer.read_string("right_name", right_name);

        if (nodes.contains(left_parent) && nodes.contains(right_parent))
        {
            auto left_pin = nodes.at(left_parent)->outPin(left_name);
            auto right_pin = nodes.at(right_parent)->inPin(right_name);
            if (left_pin && right_pin)
                right_pin->createLink(left_pin);
        }

        serializer.unload_object();
    }

    this->reload();
}

bool titian::NodeScript::is_valid() const
{
    return true;
}

void titian::NodeScript::reload()
{
}

void titian::NodeScript::call_start(Scene& scene)
{
    on_start_node->call();
}

void titian::NodeScript::call_update(Scene& scene)
{
    on_update_node->call();
}

void titian::NodeScript::call_collision(Scene& scene, Entity& attacker, Entity& target)
{
    *reinterpret_cast<Entity**>(on_collision_node->user_data + 0) = &attacker;
    *reinterpret_cast<Entity**>(on_collision_node->user_data + 8) = &target;
    on_collision_node->call();
}

void titian::NodeScript::call_ui(Scene& scene)
{
    on_ui_node->call();
}

void titian::NodeScript::update_editor()
{
    m_editor.update();
}

void titian::NodeScript::new_node_popup()
{
    String filter = gui_input_continuous("Name###NewNode");
    if (filter.empty())
    {
        for (auto& [tab_name, tab] : ui_node_generators)
        {
            if (im::BeginMenu(tab_name.data()))
            {
                for (auto& [node_name, generator] : tab)
                {
                    if (im::Selectable(node_name.data()))
                    {
                        m_editor.insertNode(m_editor.screen2grid(im::GetMousePos()), generator(this));
                        im::CloseCurrentPopup();
                    }
                }
                im::EndMenu();
            }
        }
    }
    else
    {
        for (auto& [_, tab] : ui_node_generators)
        {
            for (auto& [node_name, generator] : tab)
            {
                if (!str_find(node_name, filter))
                    continue;

                if (im::Selectable(node_name.data()))
                {
                    m_editor.insertNode(m_editor.screen2grid(im::GetMousePos()), generator(this));
                    im::CloseCurrentPopup();
                }
            }
        }
    }
}
