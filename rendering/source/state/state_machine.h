#pragma once

#include "klib.h"


// Constants
inline const std::string resouce_folder = "../resource/";
inline constexpr int shadow_map_resolution = 4096;

// Machine
struct state_machine
{
	// System
	kl::ref<kl::window> window = {};
	kl::ref<kl::gpu>       gpu = {};
	kl::timer            timer = {};

	// Graphics states
	std::unordered_map<std::string, kl::dx::depth_state>     depth_states = {};
	std::unordered_map<std::string, kl::dx::sampler_state> sampler_states = {};
	std::unordered_map<std::string, kl::dx::raster_state>   raster_states = {};

	// Graphics buffers
	std::unordered_map<std::string, kl::shaders>          shaders = {};
	std::unordered_map<std::string, kl::dx::buffer>        meshes = {};
	std::unordered_map<std::string, kl::dx::shader_view> textures = {};
	std::unordered_map<std::string, kl::dx::shader_view> skyboxes = {};

	// Graphics views
	std::unordered_map<std::string, kl::dx::target_view> target_views = {};
	std::unordered_map<std::string, kl::dx::depth_view>   depth_views = {};

	// Scene
	kl::ref<kl::scene> scene = {};
};
