#include "titian.h"


titian::Shader::Shader(const ShaderType type)
	: shader_type(type)
{
	switch (type)
	{
		case ShaderType::MATERIAL: data_buffer = get_default_material_shader(); break;
		case ShaderType::CAMERA: data_buffer = get_default_camera_shader(); break;
	}
}

void titian::Shader::serialize(Serializer& serializer) const
{
	serializer.write_int("shader_type", shader_type);
	serializer.write_string("data_buffer", data_buffer);
}

void titian::Shader::deserialize(const Serializer& serializer)
{
	serializer.read_int("shader_type", (int32_t&) shader_type);
	serializer.read_string("data_buffer", data_buffer);
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
	if (data_buffer.empty()) {
		graphics_buffer = {};
		return;
	}

	StringStream shader_sources[2] = {};
	int shader_process_type = -1;
	for (const auto& line : kl::split_string(data_buffer, '\n')) {
		if (line.find("#define _CUSTOM_VERTEX_SHADER") != -1) {
			shader_process_type = 0;
		}
		if (line.find("#define _CUSTOM_PIXEL_SHADER") != -1) {
			shader_process_type = 1;
		}
		if (shader_process_type >= 0) {
			shader_sources[shader_process_type] << line << '\n';
		}
	}

	StringStream full_source;
	for (const auto& line : kl::split_string(kl::read_file("package/shaders/scene_pass.hlsl"), '\n')) {
		if (line.find("#define _CUSTOM_VERTEX_SHADER_PLACEHOLDER") != -1) {
			full_source << shader_sources[0].str() << '\n';
		}
		else if (line.find("#define _CUSTOM_PIXEL_SHADER_PLACEHOLDER") != -1) {
			full_source << shader_sources[1].str() << '\n';
		}
		else {
			full_source << line << '\n';
		}
	}

	const Vector<dx::LayoutDescriptor> vertex_layout_descriptors = {
		{    "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{     "KL_Texture", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{      "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_BoneIndices", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_BoneWeights", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	graphics_buffer = gpu.create_render_shaders(full_source.str(), vertex_layout_descriptors);
}

void titian::Shader::reload_for_camera()
{
	if (data_buffer.empty()) {
		graphics_buffer = {};
		return;
	}
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;
	graphics_buffer = gpu.create_render_shaders(data_buffer);
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
		"VS_CB",
		"PS_CB",
		"GLOBAL_CB",
	};

	HLSL_MEMBERS = {
		"SHADOW_CASCADE_COUNT",
		"ELAPSED_TIME",
		"DELTA_TIME",
		"CAMERA_POSITION",
		"CAMERA_HAS_SKYBOX",
		"CAMERA_BACKGROUND",
		"AMBIENT_COLOR",
		"AMBIENT_INTENSITY",
		"SUN_DIRECTION",
		"SUN_POINT_SIZE",
		"SUN_COLOR",
		"OBJECT_INDEX",
		"OBJECT_SCALE",
		"OBJECT_ROTATION",
		"OBJECT_POSITION",
		"MATERIAL_COLOR",
		"ALPHA_BLEND",
		"TEXTURE_BLEND",
		"REFLECTION_FACTOR",
		"REFRACTION_FACTOR",
		"REFRACTION_INDEX",
		"HAS_NORMAL_TEXTURE",
		"HAS_ROUGHNESS_TEXTURE",
		"W",
		"V",
		"VP",
		"RECEIVES_SHADOWS",
		"SHADOW_TEXTURE_SIZE",
		"SHADOW_TEXTURE_TEXEL_SIZE",
		"SHADOW_CASCADES",
		"LIGHT_VPs",
		"BONES_BUFFER",
		"IS_SKELETAL",
		"CUSTOM_DATA",
		"SKYBOX_SAMPLER",
		"SKYBOX_TEXTURE",
		"SHADOW_SAMPLER",
		"SHADOW_TEXTURE0",
		"SHADOW_TEXTURE1",
		"SHADOW_TEXTURE2",
		"SHADOW_TEXTURE3",
		"MATERIAL_SAMPLER",
		"COLOR_TEXTURE",
		"NORMAL_TEXTURE",
		"ROUGHNESS_TEXTURE",
		"screen",
		"world",
		"textur",
		"normal",
		"light_coords",

		"FRAME_SIZE",
		"CUSTOM_DATA",
		"COLOR_TEXTURE",
		"DEPTH_TEXTURE",
		"INDEX_TEXTURE",
		"LINEAR_SAMPLER",
		"NON_LINEAR_SAMPLER",
		"position",
		"textur",
	};

	HLSL_FUNCTIONS = {
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

		"_vertex_pre",
		"_vertex_post",
		"_pixel_pre",
		"_pixel_post",
		"v_shader",
		"p_shader",
	};

	return 0;
}();
