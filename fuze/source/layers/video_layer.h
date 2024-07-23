#pragma once

#include "track/track.h"


namespace titian {
	class VideoLayer : public Layer
	{
	public:
		Vector<Ref<Track>> tracks;
		float current_time = 0.0f;

		bool timeline_seconds = true;
		bool timeline_10seconds = true;
		bool timeline_minutes = true;

		VideoLayer();

		void init();
		bool update() override;
	};
}
