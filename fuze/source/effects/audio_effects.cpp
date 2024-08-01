#include "fuze.h"


/* AudioEffectVolume */
titian::AudioEffectVolume::AudioEffectVolume()
{}

titian::String titian::AudioEffectVolume::get_name() const
{
	return "Volume";
}

void titian::AudioEffectVolume::display_gui()
{
	im::DragFloat("Volume", &volume, 0.01f);
}

void titian::AudioEffectVolume::apply(const EffectPackage& package, Audio& audio)
{
	kl::async_for(0, (int) audio.out_audio.size(), [&](const int i)
	{
		audio.out_audio[i] *= volume;
	});
}
