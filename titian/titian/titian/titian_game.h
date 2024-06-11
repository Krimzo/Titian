#pragma once

#include "packager.h"
#include "layer/layer_stack.h"

#include "application/app_layer.h"
#include "editor/game_layer.h"
#include "render/render_layer.h"
#include "gui/gui_layer.h"

#include "render/pass/shadow_pass.h"
#include "render/pass/skybox_pass.h"
#include "render/pass/scene_pass.h"
#include "render/pass/display_pass.h"


namespace titian {
	class TitianGame : public LayerStack
	{
	public:
        AppLayer app_layer = {};
        GameLayer game_layer = {};
        RenderLayer render_layer = {};
        GUILayer gui_layer = {};

        TitianGame(const std::string& entry_scene);
        ~TitianGame() override;

        bool is_valid() const;

    private:
        bool m_is_valid = true;
	};
}
