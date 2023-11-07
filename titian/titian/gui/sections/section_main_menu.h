#pragma once

#include "gui/gui_section.h"
#include "render/render_layer.h"
#include "editor/editor_layer.h"
#include "serialization/serializer.h"


namespace titian {
	class GUISectionMainMenu : public GUISection
	{
    public:
        RenderLayer* render_layer = nullptr;

        GUISectionMainMenu(RenderLayer* render_layer);

        void render_gui() override;

    private:
        bool m_inputting_name = false;
	};
}
