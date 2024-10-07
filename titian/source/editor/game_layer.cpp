#include "titian.h"


titian::GameLayer::GameLayer()
	: Layer("GameLayer")
{}

void titian::GameLayer::init()
{
	reset_scene();
}

bool titian::GameLayer::update()
{
	const TimeBomb _ = bench_time_bomb();

	if (game_running && !game_paused) {
		const float delta_time = Layers::get<AppLayer>().timer.delta();
		scene->update_physics(delta_time);
		scene->update_scripts();
	}
	return true;
}

void titian::GameLayer::reset_scene()
{
	scene = new Scene();
}

void titian::GameLayer::start_game()
{
	AppLayer& app_layer = Layers::get<AppLayer>();

	for (auto& [_, shader] : scene->shaders) {
		shader->reload();
	}
	for (auto& [_, script] : scene->scripts) {
		script->reload();
	}

	app_layer.timer.restart();
	for (auto& [_, script] : scene->scripts) {
		script->call_start(*scene);
	}

	game_running = true;
	game_paused = false;
	Logger::log("Game started.");
	app_layer.timer.restart();
}

void titian::GameLayer::pause_game()
{
	Layers::get<AppLayer>().timer.pause();
	game_paused = true;
}

void titian::GameLayer::resume_game()
{
	game_paused = false;
	Layers::get<AppLayer>().timer.resume();
}

void titian::GameLayer::stop_game()
{
	Layers::get<AppLayer>().timer.stop();
	game_running = false;
	game_paused = false;
	Logger::log("Game stopped.");
}
