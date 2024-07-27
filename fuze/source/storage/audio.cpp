#include "fuze.h"


titian::Audio::Audio()
{}

titian::Audio::Audio(const String& path)
{
	audio.load_from_file(path);
}

float titian::Audio::duration() const
{
	return audio.duration_seconds();
}

float titian::Audio::sample(const float time) const
{
	const int index = audio.sample_index(time);
	if (index >= 0 && index < (int) audio.size()) {
		return audio[index];
	}
	return 0.0f;
}
