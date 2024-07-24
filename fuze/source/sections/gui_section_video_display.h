#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionVideoDisplay : public GUISection
    {
    public:
        GUISectionVideoDisplay();

        void render_gui() override;

    private:
        Frame m_frame{};

        void clear_frame();
    };
}
