#include "fuze.h"


titian::AudioEffectVolume::AudioEffectVolume()
{}

void titian::AudioEffectVolume::apply( EffectPackage const& package, Audio& audio )
{
    kl::async_for( 0, (int) audio.out_audio.size(), [&]( int i )
    {
        audio.out_audio[i] *= volume;
    } );
}

titian::String titian::AudioEffectVolume::name() const
{
    return "Volume";
}

titian::Ref<titian::AudioEffect> titian::AudioEffectVolume::make_copy() const
{
    Ref effect = new AudioEffectVolume();
    effect->volume = this->volume;
    return effect;
}

void titian::AudioEffectVolume::display_gui()
{
    im::DragFloat( "Volume", &volume, 0.01f );
}
