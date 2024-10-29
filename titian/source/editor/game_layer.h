#pragma once

#include "app/app_layer.h"
#include "scene/scene.h"


namespace titian {
	enum struct GameState
	{
		STOPPED = 0,
		PAUSED,
		RUNNING,
	};
}

namespace titian {
	struct GameLayer : Layer_T<GameLayer>
	{
		GameLayer();

		bool update() override;

		void reset_scene();
		Scene& scene();

		void start_game();
		void pause_game();
		void resume_game();
		void stop_game();

		bool game_running() const;
		bool game_paused() const;

	private:
		Optional<Scene> m_scene;
		GameState m_game_state = GameState::STOPPED;
	};
}
