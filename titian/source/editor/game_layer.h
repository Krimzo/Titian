#pragma once

#include "application/app_layer.h"
#include "scene/scene.h"


namespace titian {
	struct GameLayer : Layer
	{
		bool game_running = false;
		bool game_paused = false;
		Ref<Scene> scene = nullptr;

		GameLayer();

		void init();
		bool update() override;

		void reset_scene();

		void start_game();
		void pause_game();
		void resume_game();
		void stop_game();
	};
}
