#pragma once

#include "gui/gui_section.h"
#include "editor/game_layer.h"


namespace titian {
    struct TitianEditor;
}

namespace titian {
    struct GUISectionBenchInfo : GUISection
    {
        TitianEditor& editor;

        GUISectionBenchInfo(TitianEditor& editor);

        void render_gui() override;
    };
}
