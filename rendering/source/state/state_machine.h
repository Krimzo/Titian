#pragma once

#include "klib.h"


struct state_machine
{
	// System
	kl::ref<kl::window> window = {};
	kl::ref<kl::gpu>       gpu = {};

	// States
	std::unordered_map<std::string, kl::dx::depth_state>     depth_states = {};
	std::unordered_map<std::string, kl::dx::sampler_state> sampler_states = {};

	// Buffers
	std::unordered_map<std::string, kl::shaders>          shaders = {};
	std::unordered_map<std::string, kl::dx::buffer>        meshes = {};
	std::unordered_map<std::string, kl::dx::shader_view> textures = {};
	std::unordered_map<std::string, kl::dx::shader_view> skyboxes = {};

	// Time
	kl::timer timer = {};

	// Scene
	kl::ref<kl::scene> scene = {};
};
