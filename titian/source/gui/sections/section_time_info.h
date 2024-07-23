#pragma once

#include "gui/gui_section.h"
#include "editor/game_layer.h"


namespace titian {
    class GUISectionTimeInfo : public GUISection
    {
    public:
        void* editor = nullptr;

        GUISectionTimeInfo(void* editor);

        void render_gui() override;
    };
}
