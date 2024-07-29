#include "fuze.h"


titian::Media::Media()
{}

void titian::Media::store_frame(const float time)
{
	store_raw_frame(time);
	apply_image_effects(time);
}

void titian::Media::store_audio()
{
	store_raw_audio();
	apply_audio_effects();
}

void titian::Media::store_raw_frame(const float time)
{
	if (type == MediaType::IMAGE) {
		if (image) {
			out_frame.resize(image->out_frame.size());
			out_frame.upload(image->out_frame);
		}
	}
	else if (type == MediaType::VIDEO) {
		if (video) {
			video->store_frame(time);
			out_frame.resize(video->out_frame.size());
			out_frame.upload(video->out_frame);
		}
	}
}

void titian::Media::store_raw_audio()
{
	if (type == MediaType::AUDIO) {
		if (audio) {
			out_audio.out_audio = audio->out_audio;
			out_audio.out_audio.set_duration(this->duration);
		}
	}
	else if (type == MediaType::VIDEO) {
		if (audio) {
			out_audio.out_audio = audio->out_audio;
			out_audio.out_audio.set_duration(this->duration);
		}
	}
}

void titian::Media::apply_image_effects(const float time)
{
	for (const auto& effect : image_effects)
		effect->apply(time, out_frame);
}

void titian::Media::apply_audio_effects()
{
	for (const auto& effect : audio_effects)
		effect->apply(out_audio);
}
