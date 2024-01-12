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
        bool m_is_valid = true;

	public:
        kl::Object<AppLayer> app_layer = nullptr;
        kl::Object<GameLayer> game_layer = nullptr;
        kl::Object<RenderLayer> render_layer = nullptr;
        kl::Object<GUILayer> gui_layer = nullptr;

        TitianGame(const std::string& entry_scene);
        ~TitianGame() override;

        bool is_valid() const;
	};
}
