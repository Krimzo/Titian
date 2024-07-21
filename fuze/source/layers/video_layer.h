#pragma once

#include "track/track.h"


namespace fuze {
	class VideoLayer : public Layer
	{
	public:
		Vector<Ref<Track>> tracks;

		VideoLayer();

		void init();
		bool update() override;
	};
}
