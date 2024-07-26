#include "fuze.h"


titian::Media::Media()
{}

bool titian::Media::get_frame(const float time)
{
	if (!get_raw_frame(time, out_frame)) {
		return false;
	}
	apply_image_effects(time, out_frame);
	return true;
}

void titian::Media::get_audio()
{
	get_raw_audio(out_audio);
	apply_audio_effects(out_audio);
}

bool titian::Media::get_raw_frame(const float time, Frame& out)
{
	if (type == MediaType::IMAGE) {
		if (image) {
			out.resize(image->size());
			out.upload(*image);
			return true;
		}
	}
	else if (type == MediaType::VIDEO) {
		if (video) {
			Image temp;
			if (video->get_frame(time, temp)) {
				out.resize(temp.size());
				out.upload(temp);
				return true;
			}
		}
	}
	return false;
}

void titian::Media::get_raw_audio(Audio& out)
{
	if (type == MediaType::AUDIO) {
		if (audio) {
			out = *audio;
		}
	}
	else if (type == MediaType::VIDEO) {
		if (audio) {
			out = *audio;
		}
	}
}

void titian::Media::apply_image_effects(const float time, Frame& out) const
{
	for (const auto& effect : image_effects)
		effect->apply(time, out);
}

void titian::Media::apply_audio_effects(Audio& out) const
{
	for (const auto& effect : audio_effects)
		effect->apply(out);
}
