#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace fuze {
    using namespace titian;
}

namespace fuze {
    class GUISectionVideoTimeline : public GUISection
    {
    public:
        int vertical_offset = 0;
        float horizontal_offset = 0.0f;

        int vertical_view = 5;
        float horizontal_view = 60.0f;

        GUISectionVideoTimeline(const LayerPackage& package);

        void render_gui() override;

    private:
        int m_last_scroll = 0;
    };
}
