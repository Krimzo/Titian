#pragma once

#include "gui/gui_section.h"
#include "editor/game_layer.h"


namespace titian {
    struct GUISectionBenchInfo : GUISection
    {
        void* editor = nullptr;

        GUISectionBenchInfo(void* editor);

        void render_gui() override;
    };
}
