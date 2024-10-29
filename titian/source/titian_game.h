#pragma once

#include "packager.h"
#include "layer/layer_stack.h"

#include "app/app_layer.h"
#include "editor/game_layer.h"
#include "render/render_layer.h"
#include "gui/gui_layer.h"

#include "render/pass/shadow_pass.h"
#include "render/pass/skybox_pass.h"
#include "render/pass/scene_pass.h"
#include "render/pass/post_pass.h"
#include "render/pass/display_pass.h"

#include "gui/ui/ui_funcs.h"


namespace titian {
    struct TitianGame : LayerStack
	{
        AppLayer& app_layer;
        GameLayer& game_layer;
        RenderLayer& render_layer;
        GUILayer& gui_layer;

        TitianGame(const StringView& entry_scene);
        ~TitianGame() override;
	};
}
