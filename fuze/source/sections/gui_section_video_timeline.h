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
        struct MovingMediaInfo
        {
            int track_index = 0;
            Ref<Media> media;
            float horiz_offset = 0.0f;
            float offset = 0.0f;
            ImVec2 old_mouse = {};
            ImVec2 mouse_offset = {};
        };

        int m_last_scroll = 0;
        Optional<MovingMediaInfo> m_moving_media;

        static ImColor color_classify(MediaType type);
    };
}
