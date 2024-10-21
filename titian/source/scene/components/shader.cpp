#include "titian.h"


titian::Shader::Shader(const ShaderType type)
	: shader_type(type)
{
	switch (type)
	{
	case ShaderType::MATERIAL:
		source = get_default_material_shader(); break;
	case ShaderType::CAMERA:
		source = get_default_camera_shader(); break;
	}
}

void titian::Shader::serialize(Serializer& serializer) const
{
	serializer.write_int("shader_type", shader_type);
	serializer.write_string("source", source);
}

void titian::Shader::deserialize(const Serializer& serializer)
{
	serializer.read_int("shader_type", (int32_t&) shader_type);
	serializer.read_string("source", source);
	reload();
}

void titian::Shader::reload()
{
	if (shader_type == ShaderType::MATERIAL) {
		reload_for_material();
	}
	else if (shader_type == ShaderType::CAMERA) {
		reload_for_camera();
	}
}

void titian::Shader::reload_for_material()
{
	if (source.empty()) {
		shaders = {};
		return;
	}
	static const Vector<dx::LayoutDescriptor> vertex_layout_descriptors = {
		{ "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_BoneIndices", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_BoneWeights", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	shaders = gpu.create_shaders(source, vertex_layout_descriptors);
}

void titian::Shader::reload_for_camera()
{
	if (source.empty()) {
		shaders = {};
		return;
	}
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	shaders = gpu.create_shaders(source);
}

static const int load_names = [&]
{
	using namespace titian;

	HLSL_KEYWORDS = {
		"AppendStructuredBuffer", "asm", "asm_fragment", "BlendState", "bool", "break", "Buffer", "ByteAddressBuffer", "case", "cbuffer", "centroid", "class", "column_major", "compile", "compile_fragment",
		"CompileShader", "const", "continue", "ComputeShader", "ConsumeStructuredBuffer", "default", "DepthStencilState", "DepthStencilView", "discard", "do", "double", "DomainShader", "dword", "else",
		"export", "extern", "false", "float", "for", "fxgroup", "GeometryShader", "groupshared", "half", "Hullshader", "if", "in", "inline", "inout", "InputPatch", "int", "interface", "line", "lineadj",
		"linear", "LineStream", "matrix", "min16float", "min10float", "min16int", "min12int", "min16uint", "namespace", "nointerpolation", "noperspective", "NULL", "out", "OutputPatch", "packoffset",
		"pass", "pixelfragment", "PixelShader", "point", "PointStream", "precise", "RasterizerState", "RenderTargetView", "return", "register", "row_major", "RWBuffer", "RWByteAddressBuffer", "RWStructuredBuffer",
		"RWTexture1D", "RWTexture1DArray", "RWTexture2D", "RWTexture2DArray", "RWTexture3D", "sample", "sampler", "SamplerState", "SamplerComparisonState", "shared", "snorm", "stateblock", "stateblock_state",
		"static", "string", "struct", "switch", "StructuredBuffer", "tbuffer", "technique", "technique10", "technique11", "texture", "Texture1D", "Texture1DArray", "Texture2D", "Texture2DArray", "Texture2DMS",
		"Texture2DMSArray", "Texture3D", "TextureCube", "TextureCubeArray", "true", "typedef", "triangle", "triangleadj", "TriangleStream", "uint", "uniform", "unorm", "unsigned", "vector", "vertexfragment",
		"VertexShader", "void", "volatile", "while",
		"bool1","bool2","bool3","bool4","double1","double2","double3","double4", "float1", "float2", "float3", "float4", "int1", "int2", "int3", "int4", "in", "out", "inout",
		"uint1", "uint2", "uint3", "uint4", "dword1", "dword2", "dword3", "dword4", "half1", "half2", "half3", "half4",
		"float1x1","float2x1","float3x1","float4x1","float1x2","float2x2","float3x2","float4x2",
		"float1x3","float2x3","float3x3","float4x3","float1x4","float2x4","float3x4","float4x4",
		"half1x1","half2x1","half3x1","half4x1","half1x2","half2x2","half3x2","half4x2",
		"half1x3","half2x3","half3x3","half4x3","half1x4","half2x4","half3x4","half4x4",
	};

	HLSL_TYPES = {
		"VS_OUT",
		"PS_OUT",
		"GS_OUT",
		"CS_OUT",
	};

	HLSL_MEMBERS.insert({
		"SHADOW_CASCADE_COUNT",
		"W",
		"V",
		"VP",
		"LIGHT_VPs",
		"CUSTOM_DATA",
		"CAMERA_BACKGROUND",
		"MATERIAL_COLOR",
		"SHADOW_CASCADES",
		"CAMERA_POSITION",
		"CAMERA_HAS_SKYBOX",
		"AMBIENT_COLOR",
		"RECEIVES_SHADOWS",
		"SUN_DIRECTION",
		"SUN_POINT_SIZE",
		"SUN_COLOR",
		"ELAPSED_TIME",
		"OBJECT_SCALE",
		"DELTA_TIME",
		"OBJECT_ROTATION",
		"OBJECT_INDEX",
		"OBJECT_POSITION",
		"TEXTURE_BLEND",
		"SHADOW_TEXTURE_SIZE",
		"SHADOW_TEXTURE_TEXEL_SIZE",
		"REFLECTIVITY_FACTOR",
		"REFRACTION_INDEX",
		"HAS_NORMAL_TEXTURE",
		"HAS_ROUGHNESS_TEXTURE",
		"IS_SKELETAL",
		"BONES_BUFFER",
		"SKYBOX_TEXTURE",
		"SHADOW_TEXTURE0",
		"SHADOW_TEXTURE1",
		"SHADOW_TEXTURE2",
		"SHADOW_TEXTURE3",
		"COLOR_TEXTURE",
		"NORMAL_TEXTURE",
		"ROUGHNESS_TEXTURE",
		"SKYBOX_SAMPLER",
		"SHADOW_SAMPLER",
		"MATERIAL_SAMPLER",
	});

	HLSL_MEMBERS.insert({
		"CUSTOM_DATA",
		"FRAME_SIZE",
		"COLOR_TEXTURE",
		"DEPTH_TEXTURE",
		"INDEX_TEXTURE",
		"LINEAR_SAMPLER",
		"NON_LINEAR_SAMPLER",
	});

	HLSL_FUNCTIONS.insert({
		"abort", "abs", "acos", "all", "AllMemoryBarrier", "AllMemoryBarrierWithGroupSync", "any", "asdouble", "asfloat", "asin", "asint", "asint", "asuint",
		"asuint", "atan", "atan2", "ceil", "CheckAccessFullyMapped", "clamp", "clip", "cos", "cosh", "countbits", "cross", "D3DCOLORtoUBYTE4", "ddx",
		"ddx_coarse", "ddx_fine", "ddy", "ddy_coarse", "ddy_fine", "degrees", "determinant", "DeviceMemoryBarrier", "DeviceMemoryBarrierWithGroupSync",
		"distance", "dot", "dst", "errorf", "EvaluateAttributeAtCentroid", "EvaluateAttributeAtSample", "EvaluateAttributeSnapped", "exp", "exp2",
		"f16tof32", "f32tof16", "faceforward", "firstbithigh", "firstbitlow", "floor", "fma", "fmod", "frac", "frexp", "fwidth", "GetRenderTargetSampleCount",
		"GetRenderTargetSamplePosition", "GroupMemoryBarrier", "GroupMemoryBarrierWithGroupSync", "InterlockedAdd", "InterlockedAnd", "InterlockedCompareExchange",
		"InterlockedCompareStore", "InterlockedExchange", "InterlockedMax", "InterlockedMin", "InterlockedOr", "InterlockedXor", "isfinite", "isinf", "isnan",
		"ldexp", "length", "lerp", "lit", "log", "log10", "log2", "mad", "max", "min", "modf", "msad4", "mul", "noise", "normalize", "pow", "printf",
		"Process2DQuadTessFactorsAvg", "Process2DQuadTessFactorsMax", "Process2DQuadTessFactorsMin", "ProcessIsolineTessFactors", "ProcessQuadTessFactorsAvg",
		"ProcessQuadTessFactorsMax", "ProcessQuadTessFactorsMin", "ProcessTriTessFactorsAvg", "ProcessTriTessFactorsMax", "ProcessTriTessFactorsMin",
		"radians", "rcp", "reflect", "refract", "reversebits", "round", "rsqrt", "saturate", "sign", "sin", "sincos", "sinh", "smoothstep", "sqrt", "step",
		"tan", "tanh", "tex1D", "tex1D", "tex1Dbias", "tex1Dgrad", "tex1Dlod", "tex1Dproj", "tex2D", "tex2D", "tex2Dbias", "tex2Dgrad", "tex2Dlod", "tex2Dproj",
		"tex3D", "tex3D", "tex3Dbias", "tex3Dgrad", "tex3Dlod", "tex3Dproj", "texCUBE", "texCUBE", "texCUBEbias", "texCUBEgrad", "texCUBElod", "texCUBEproj", "transpose", "trunc",
	});

	HLSL_FUNCTIONS.insert({
		"v_shader",
		"p_shader",
	});

	return 0;
}();
