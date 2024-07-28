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
	return out_audio.sample_at_time(time);
}
