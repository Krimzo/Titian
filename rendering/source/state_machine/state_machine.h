#pragma once

#include "scene/scene.h"


struct state_machine
{
	// System
	kl::ref<kl::window> window = {};
	kl::ref<kl::gpu> gpu = {};

	// Buffers
	std::unordered_map<std::string, kl::shaders>          shaders = {};
	std::unordered_map<std::string, kl::dx::buffer>        meshes = {};
	std::unordered_map<std::string, kl::dx::shader_view> textures = {};

	// Time
	kl::timer timer = {};

	// Scene
	kl::ref<scene> scene = {};
};
