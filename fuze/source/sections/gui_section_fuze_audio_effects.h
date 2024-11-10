#pragma once

#include "gui/gui_section.h"
#include "gui/gui_layer.h"


namespace titian
{
struct GUISectionFuzeAudioEffects : GUISection
{
    GUISectionFuzeAudioEffects();

    void render_gui() override;
};
}
