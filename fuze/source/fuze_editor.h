#pragma once

#include "titian.h"
#include "layers/video_layer.h"

#include "sections/gui_section_fuze_main_menu.h"
#include "sections/gui_section_fuze_image_effects.h"
#include "sections/gui_section_fuze_audio_effects.h"
#include "sections/gui_section_fuze_display.h"
#include "sections/gui_section_fuze_timeline.h"
#include "sections/gui_section_fuze_render.h"


namespace titian {
    struct FuzeEditor : LayerStack
    {
        AppLayer app_layer = {};
        VideoLayer video_layer = {};
        GUILayer gui_layer = {};

        FuzeEditor();
        ~FuzeEditor() override;
    };
}
