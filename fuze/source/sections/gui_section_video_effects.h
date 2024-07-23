#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace fuze {
    using namespace titian;
}

namespace fuze {
    class GUISectionVideoEffects : public GUISection
    {
    public:
        GUISectionVideoEffects();

        void render_gui() override;
    };
}
