#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian
{
struct GUISectionFuzeDisplay : GUISection
{
    GUISectionFuzeDisplay();

    void render_gui() override;
};
}
