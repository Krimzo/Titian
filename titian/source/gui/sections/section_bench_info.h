#pragma once

#include "gui/gui_section.h"
#include "editor/game_layer.h"


namespace titian {
    class GUISectionBenchInfo : public GUISection
    {
    public:
        void* editor = nullptr;

        GUISectionBenchInfo(void* editor);

        void render_gui() override;
    };
}
