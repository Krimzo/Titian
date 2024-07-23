#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionVideoMainMenu : public GUISection
    {
    public:
        GUISectionVideoMainMenu();

        void render_gui() override;

    private:
        bool m_testing_exit = false;
    };
}
