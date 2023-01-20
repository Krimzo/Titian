#pragma once

#include "scene/scene.h"


struct state_machine
{
	// System
	kl::window* window = nullptr;
	kl::gpu* gpu = nullptr;

	// Buffers
	std::unordered_map<std::string, kl::shaders>          shaders = {};
	std::unordered_map<std::string, kl::dx::buffer>        meshes = {};
	std::unordered_map<std::string, kl::dx::shader_view> textures = {};

	// Time
	kl::timer timer = {};

	// Scene
	kl::ref<scene> scene = {};
};
