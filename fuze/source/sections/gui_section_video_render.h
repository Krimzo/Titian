#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionVideoRender : public GUISection
    {
    public:
        GUISectionVideoRender();

        void render_gui() override;
    };
}
