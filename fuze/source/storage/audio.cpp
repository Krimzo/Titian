#include "fuze.h"


titian::Audio::Audio()
{}

titian::Audio::Audio(const String& path)
{
	out_audio.load_from_file(path);
}

float titian::Audio::duration() const
{
	return out_audio.duration_seconds();
}

float titian::Audio::sample(const float time) const
{
	const int index = out_audio.sample_index(time);
	if (index >= 0 && index < (int) out_audio.size()) {
		return out_audio[index];
	}
	return 0.0f;
}
