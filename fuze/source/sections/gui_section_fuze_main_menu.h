#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionFuzeMainMenu : public GUISection
    {
    public:
        GUISectionFuzeMainMenu();

        void render_gui() override;

    private:
        bool m_testing_exit = false;
    };
}