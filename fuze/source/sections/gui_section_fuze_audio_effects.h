#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionFuzeAudioEffects : public GUISection
    {
    public:
        GUISectionFuzeAudioEffects();

        void render_gui() override;
    };
}
