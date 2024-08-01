#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionFuzeDisplay : public GUISection
    {
    public:
        GUISectionFuzeDisplay();

        void render_gui() override;
    };
}
