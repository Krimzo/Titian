#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionFuzeRender : public GUISection
    {
    public:
        GUISectionFuzeRender();

        void render_gui() override;
    };
}
