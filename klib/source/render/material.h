#pragma once

#include "math/vector/vector4.h"
#include "graphics/dx_types.h"


namespace kl {
	class material
	{
	public:
		float4 color = {};

		dx::shader_view color_map = nullptr;
		dx::shader_view normal_map = nullptr;
		dx::shader_view depth_map = nullptr;

		float texture_blend = 0.0f;
		float reflection_factor = 0.0f;
		float refraction_factor = 0.0f;
		float refraction_index = 0.0f;
		
		material();
		~material();
	};
}
