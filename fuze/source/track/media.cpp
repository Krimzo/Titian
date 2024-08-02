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

void titian::Media::store_frame(const EffectPackage& package, const bool wait)
{
	store_raw_frame(package.current_time - package.media_start, wait);
	apply_image_effects(package);
}

void titian::Media::store_audio(const EffectPackage& package)
{
	store_raw_audio();
	apply_audio_effects(package);
}

titian::Ref<titian::Media> titian::Media::make_copy() const
{
	Ref media = new Media();
	media->name = this->name;
	media->type = this->type;
	media->duration = this->duration;
	media->image = this->image ? this->image->make_copy() : nullptr;
	media->audio = this->audio ? this->audio->make_copy() : nullptr;
	media->video = this->video ? this->video->make_copy() : nullptr;
	for (const auto& effect : this->image_effects) {
		media->image_effects.push_back(effect->make_copy());
	}
	for (const auto& effect : this->audio_effects) {
		media->audio_effects.push_back(effect->make_copy());
	}
	return media;
}

void titian::Media::store_raw_frame(const float time, const bool wait)
{
	if (type == MediaType::IMAGE) {
		if (image) {
			out_frame.resize(image->out_frame.size());
			out_frame.upload(image->out_frame);
		}
	}
	else if (type == MediaType::VIDEO) {
		if (video) {
			video->store_frame(time, wait);
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
