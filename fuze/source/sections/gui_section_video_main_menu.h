#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace fuze {
    using namespace titian;
}

namespace fuze {
    class GUISectionVideoMainMenu : public GUISection
    {
    public:
        GUISectionVideoMainMenu(const LayerPackage& package);

        void render_gui() override;

    private:
        bool m_testing_exit = false;
    };
}
