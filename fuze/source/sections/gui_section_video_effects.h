#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionVideoEffects : public GUISection
    {
    public:
        GUISectionVideoEffects();

        void render_gui() override;
    };
}
