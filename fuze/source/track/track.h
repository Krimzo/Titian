#pragma once

#include "track/media.h"


namespace fuze {
	class Track
	{
	public:
		String name = "Track";
		bool enabled = true;
		Vector<Ref<Media>> media;

		Track();
	};
}
