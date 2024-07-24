#include "fuze.h"


titian::Media::Media()
{}

bool titian::Media::get_frame(const float time, Frame& out_frame) const
{
	if (!get_raw_frame(time, out_frame)) {
		return false;
	}
	apply_image_effects(time, out_frame);
	return true;
}

bool titian::Media::get_audio(const float time, const float duration, Audio& out_audio) const
{
	if (!get_raw_audio(time, duration, out_audio)) {
		return false;
	}
	apply_audio_effects(time, duration, out_audio);
	return true;
}

bool titian::Media::get_raw_frame(const float time, Frame& out_frame) const
{
	if (type == MediaType::IMAGE) {
		if (image) {
			out_frame.upload(*image);
			return true;
		}
	}
	else if (type == MediaType::VIDEO) {
		if (video) {
			Image temp;
			if (video->get_frame(time, temp)) {
				out_frame.upload(temp);
				return true;
			}
		}
	}
	return false;
}

bool titian::Media::get_raw_audio(const float time, const float duration, Audio& out_audio) const
{
	if (type == MediaType::AUDIO) {
		if (audio) {
			return audio->get_audio(time, duration, out_audio);
		}
	}
	else if (type == MediaType::VIDEO) {
		if (audio) {
			return audio->get_audio(time, duration, out_audio);
		}
	}
	return false;
}

void titian::Media::apply_image_effects(const float time, Frame& out_frame) const
{
	for (const auto& effect : image_effects)
		effect->apply(time, out_frame);
}

void titian::Media::apply_audio_effects(const float time, const float duration, Audio& out_audio) const
{
	for (const auto& effect : audio_effects)
		effect->apply(time, duration, out_audio);
}
