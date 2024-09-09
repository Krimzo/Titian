#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian {
    struct GUISectionFuzeRender : GUISection
    {
        Int2 video_resolution = { 1920, 1080 };
        int video_fps = 60;
        int video_bitrate = 10'000'000;
        int audio_rate = 48000;

        GUISectionFuzeRender();

        void render_gui() override;
    };
}
