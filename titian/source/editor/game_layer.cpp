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
	if (m_game_running && !m_game_paused) {
		const float delta_time = Layers::get<AppLayer>().timer.delta();
		m_scene->update_physics(delta_time);
		m_scene->update_scripts();
	}
	return true;
}

void titian::GameLayer::reset_scene()
{
	m_scene.emplace();
}

titian::Scene& titian::GameLayer::scene()
{
	return *m_scene;
}

void titian::GameLayer::start_game()
{
	AppLayer& app_layer = Layers::get<AppLayer>();

	for (auto& [_, shader] : m_scene->shaders) {
		shader->reload();
	}
	for (auto& [_, script] : m_scene->scripts) {
		script->reload();
	}

	app_layer.timer.restart();
	for (auto& [_, script] : m_scene->scripts) {
		script->call_start(*m_scene);
	}

	m_game_running = true;
	m_game_paused = false;
	Logger::log("Game started.");
	app_layer.timer.restart();
}

void titian::GameLayer::pause_game()
{
	Layers::get<AppLayer>().timer.pause();
	m_game_paused = true;
}

void titian::GameLayer::resume_game()
{
	m_game_paused = false;
	Layers::get<AppLayer>().timer.resume();
}

void titian::GameLayer::stop_game()
{
	Layers::get<AppLayer>().timer.stop();
	m_game_running = false;
	m_game_paused = false;
	Logger::log("Game stopped.");
}

bool titian::GameLayer::game_running() const
{
	return m_game_running;
}

bool titian::GameLayer::game_paused() const
{
	return m_game_paused;
}
