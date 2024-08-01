#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"

#include "effects/image_effects.h"
#include "effects/audio_effects.h"


namespace titian {
    class GUISectionFuzeImageEffects : public GUISection
    {
    public:
        GUISectionFuzeImageEffects();

        void render_gui() override;
    };
}
