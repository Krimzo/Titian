#pragma once

#include "app/app_layer.h"
#include "scene/scene.h"


namespace titian {
	struct GameLayer : Layer
	{
		GameLayer();

		void init();
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
		Ref<Scene> m_scene;
		bool m_game_running : 1 = false;
		bool m_game_paused : 1 = false;
	};
}
