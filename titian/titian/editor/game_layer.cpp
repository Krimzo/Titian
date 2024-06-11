#include "main.h"


titian::GameLayer::GameLayer()
	: Layer("GameLayer")
{}

void titian::GameLayer::init()
{
	scene = new Scene(&app_layer->gpu);
}

bool titian::GameLayer::update()
{
	const TimeBomb _ = this->time_it();

	if (game_running && !game_paused) {
		const float delta_time = app_layer->timer.delta();
		scene->update_physics(delta_time);
		scene->update_scripts();
	}
	return true;
}

void titian::GameLayer::reset_scene()
{
	scene = new Scene(&app_layer->gpu);
}

void titian::GameLayer::start_game()
{
	// Reload
	for (auto& [_, shader] : scene->shaders) {
		shader->reload();
	}
	for (auto& [_, script] : scene->scripts) {
		script->reload();
	}

	// Start
	app_layer->timer.reset();
	for (auto& [_, script] : scene->scripts) {
		script->call_start(&scene);
	}

	// Change state
	game_running = true;
	game_paused = false;
	Logger::log("Game started.");
	app_layer->timer.reset();
}

void titian::GameLayer::pause_game()
{
	app_layer->timer.pause();
	game_paused = true;
}

void titian::GameLayer::resume_game()
{
	game_paused = false;
	app_layer->timer.resume();
}

void titian::GameLayer::stop_game()
{
	// Change state
	game_running = false;
	game_paused = false;
	Logger::log("Game stopped.");
}
