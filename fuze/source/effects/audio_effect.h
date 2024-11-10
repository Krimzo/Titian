#pragma once

#include "effects/effect_package.h"
#include "storage/audio.h"


namespace titian
{
struct AudioEffect : kl::NoCopy
{
    AudioEffect();
    virtual ~AudioEffect() = default;

    virtual void apply( EffectPackage const& package, Audio& audio ) = 0;

    virtual String name() const = 0;
    virtual Ref<AudioEffect> make_copy() const = 0;
    virtual void display_gui() = 0;
};
}
