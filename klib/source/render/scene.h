#pragma once

#include "render/entity.h"
#include "render/camera.h"
#include "memory/ref.h"


namespace kl {
	class scene : public std::vector<kl::ref<kl::entity>>
	{
	public:
		kl::camera camera = {};
		kl::float3 sun_direction = {};

		scene();
	};
}
