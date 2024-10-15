#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    struct GUISectionFuzeTimeline : GUISection
    {
        int vertical_offset = 0;
        float horizontal_offset = 0.0f;

        int vertical_view = 5;
        float horizontal_view = 60.0f;

        GUISectionFuzeTimeline();

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

        Optional<MovingMediaInfo> m_moving_media;

        bool m_editing_track = false;
        bool m_editing_media = false;
        bool m_popup_was = false;

        void handle_input(int scroll);
        void render_header(ImVec2 cell_padding, float available_height, float& header_height, ImVec2& total_min, ImVec2& total_max);
        void render_track(ImVec2 cell_padding, float row_height, int i, float header_height, const ImVec2& total_min, ImVec2& total_max);
        void render_pointer(ImVec2 total_min, ImVec2 total_max);

        static ImColor color_classify(const Media& media);
    };
}
