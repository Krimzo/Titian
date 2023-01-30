#pragma once

#include "math/vector/vector4.h"
#include "graphics/dx_types.h"


namespace kl {
	class material
	{
	public:
		float4 color = {};
		float roughness = 1.0f;

		dx::shader_view color_map = nullptr;
		dx::shader_view normal_map = nullptr;
		dx::shader_view depth_map = nullptr;
		
		material();
		~material();
	};
}
