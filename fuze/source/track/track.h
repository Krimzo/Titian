#pragma once

#include "track/media.h"


namespace titian {
	class Track
	{
	public:
		String name = "Track";
		bool enabled = true;
		std::map<float, Ref<Media>> media;

		Track();

		Track(const Track&) = delete;
		Track(Track&&) = delete;

		void operator=(const Track&) = delete;
		void operator=(Track&&) = delete;

		void insert_media(float offset, const Ref<Media>& med);
		void remove_media(const Ref<Media>& med);

		void readjust_media();
	};
}
