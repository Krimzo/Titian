#pragma once

#include "gui/gui_section.h"
#include "editor/game_layer.h"


namespace titian {
    class GUISectionRenderInfo : public GUISection
    {
    public:
        GameLayer* game_layer = nullptr;

        int updates_per_second = 6;
        float frame_time_ms = 0.0f;
        int frames_per_second = 0;

        GUISectionRenderInfo(GameLayer* game_layer);

        void render_gui() override;

    private:
        float m_last_update_time = 0.0f;
    };
}
