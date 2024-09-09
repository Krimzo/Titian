#pragma once

#include "track/media.h"


namespace titian {
	struct Track : kl::NoCopy
	{
		String name = "Track";
		bool enabled = true;
		std::map<float, Ref<Media>> media;

		Track();

		void insert_media(float offset, const Ref<Media>& med);
		void remove_media(const Ref<Media>& med);

		Ref<Media> get_media(float time, float& out_offset) const;

		Ref<Track> make_copy() const;

	private:
		void readjust_media();
	};
}
