#include "fuze.h"


titian::Media::Media()
{}

bool titian::Media::has_image() const
{
	return image;
}

bool titian::Media::has_audio() const
{
	return audio && audio->duration() > 0.0f;
}

bool titian::Media::has_video() const
{
	return video && video->duration() > 0.0f;
}

void titian::Media::store_frame(const EffectPackage& package)
{
	store_raw_frame(package.current_time - package.media_start);
	apply_image_effects(package);
}

void titian::Media::store_audio(const EffectPackage& package)
{
	store_raw_audio();
	apply_audio_effects(package);
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

void titian::Media::apply_image_effects(const EffectPackage& package)
{
	for (auto& effect : image_effects)
		effect->apply(package, out_frame);
}

void titian::Media::apply_audio_effects(const EffectPackage& package)
{
	for (auto& effect : audio_effects)
		effect->apply(package, out_audio);
}
