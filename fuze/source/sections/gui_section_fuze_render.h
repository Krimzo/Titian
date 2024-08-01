#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionFuzeRender : public GUISection
    {
    public:
        Int2 video_resolution = { 1920, 1080 };
        int video_fps = 30;
        int video_bitrate = 40'000'000;
        int audio_rate = 96000;

        GUISectionFuzeRender();

        void render_gui() override;
    };
}
