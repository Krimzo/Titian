#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace fuze {
    using namespace titian;
}

namespace fuze {
    class GUISectionVideoDisplay : public GUISection
    {
    public:
        GUISectionVideoDisplay(const LayerPackage& package);

        void render_gui() override;
    };
}