#pragma once

#include "titian.h"
#include "layers/video_layer.h"

#include "sections/gui_section_video_main_menu.h"
#include "sections/gui_section_video_effects.h"
#include "sections/gui_section_video_display.h"
#include "sections/gui_section_video_timeline.h"
#include "sections/gui_section_video_render.h"


namespace fuze {
    using namespace titian;
}

namespace fuze {
    class FuzeEditor : public LayerStack
    {
    public:
        AppLayer app_layer = {};
        VideoLayer video_layer = {};
        GUILayer gui_layer = {};

        FuzeEditor();
        ~FuzeEditor() override;
    };
}
