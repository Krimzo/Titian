#pragma once

#include "render/entity.h"
#include "render/camera.h"
#include "memory/ref.h"


namespace kl {
	class scene : public std::vector<kl::ref<kl::entity>>
	{
	public:
		kl::camera camera = {};

		float ambient_light = 0.1f;
		kl::float3 sun_direction = {};

		kl::dx::buffer ocean_mesh = nullptr;

		scene();
	};
}
