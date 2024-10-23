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
	if (m_game_state == GameState::RUNNING) {
		m_scene->update_physics(Layers::get<AppLayer>().timer.delta());
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
	if (m_game_state != GameState::STOPPED)
		return;

	AppLayer& app_layer = Layers::get<AppLayer>();

	for (auto& [_, shader] : m_scene->shaders) {
		shader->reload();
	}
	for (auto& [_, script] : m_scene->scripts) {
		script->reload();
	}

	app_layer.timer.reset();
	for (auto& [_, script] : m_scene->scripts) {
		script->call_start(*m_scene);
	}

	app_layer.timer.start();
	m_game_state = GameState::RUNNING;
	Logger::log("Game started.");
}

void titian::GameLayer::pause_game()
{
	if (m_game_state != GameState::RUNNING)
		return;
	Layers::get<AppLayer>().timer.stop();
	m_game_state = GameState::PAUSED;
}

void titian::GameLayer::resume_game()
{
	if (m_game_state != GameState::PAUSED)
		return;
	Layers::get<AppLayer>().timer.start();
	m_game_state = GameState::RUNNING;
}

void titian::GameLayer::stop_game()
{
	if (m_game_state == GameState::STOPPED)
		return;
	Layers::get<AppLayer>().timer.reset();
	m_game_state = GameState::STOPPED;
	Logger::log("Game stopped.");
}

bool titian::GameLayer::game_running() const
{
	return m_game_state == GameState::RUNNING;
}

bool titian::GameLayer::game_paused() const
{
	return m_game_state == GameState::PAUSED;
}
