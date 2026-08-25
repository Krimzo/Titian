#pragma once

#include "gui/gui_section.h"
#include "editor/game_layer.h"


namespace titian
{
struct TitianEditor;
}

namespace titian
{
struct GUISectionBenchInfo : GUISection
{
    TitianEditor const& editor;

    GUISectionBenchInfo( TitianEditor const& editor );

    void render_gui() override;
};
}
