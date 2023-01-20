#pragma once

#include "klib.h"


class scene : public std::vector<kl::ref<kl::entity>>
{
public:
	kl::camera camera = {};
	kl::float3 sun_direction = {};

	scene();
};
