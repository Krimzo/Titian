#pragma once

#include "application/app_layer.h"
#include "scene/scene.h"


namespace titian {
	class GameLayer : public Layer
	{
	public:
		static GameLayer* BOUND_SELF;

		AppLayer* app_layer = nullptr;

		bool game_running = false;
		bool game_paused = false;
		kl::Object<Scene> scene = nullptr;

		GameLayer(AppLayer* app_layer);

		bool update() override;

		void bind_self();
		void reset_scene();

		void start_game();
		void pause_game();
		void resume_game();
		void stop_game();
	};
}
