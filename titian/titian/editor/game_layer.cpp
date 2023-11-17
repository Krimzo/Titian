#include "main.h"


titian::GameLayer* titian::GameLayer::BOUND_SELF = nullptr;

titian::GameLayer::GameLayer(AppLayer* app_layer)
{
	this->app_layer = app_layer;
	scene = new Scene(&app_layer->gpu);
}

bool titian::GameLayer::update()
{
	if (game_running) {
		const float delta_time = app_layer->timer->delta();
		scene->update_physics(delta_time);
		scene->update_scripts();
	}
	return true;
}

void titian::GameLayer::bind_self()
{
	BOUND_SELF = this;
}

void titian::GameLayer::reset_scene()
{
	scene = new Scene(&app_layer->gpu);
}

void titian::GameLayer::start_game()
{
	// Reload and call scripts
	app_layer->timer->reset_elapsed();
	for (auto& [_, script] : scene->scripts) {
		script->reload();
		script->call_start();
	}

	// Change state
	game_running = true;
	Logger::log("Game started.");
}

void titian::GameLayer::stop_game()
{
	// Change state
	game_running = false;
	Logger::log("Game stopped.");
}