#pragma once

#include "effects/image_effect.h"

namespace titian
{
struct ImageEffectSclRotPos : ImageEffect
{
    ImageEffectSclRotPos();

    String name() const override;
    bool needs_copy() const override;
    Ref<ImageEffect> make_copy() const override;
    void display_gui() override;

    String get_source() const;

    float2& scale();
    float& rotation();
    float2& position();
};
} // namespace titian
