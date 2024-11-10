#pragma once

#include "effects/audio_effect.h"


namespace titian
{
struct AudioEffectVolume : AudioEffect
{
    float volume = 1.0f;

    AudioEffectVolume();

    void apply( EffectPackage const& package, Audio& audio ) override;

    String name() const override;
    Ref<AudioEffect> make_copy() const override;
    void display_gui() override;
};
}
