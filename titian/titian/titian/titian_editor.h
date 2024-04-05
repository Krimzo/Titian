#pragma once

#include "packager.h"
#include "layer/layer_stack.h"

#include "application/app_layer.h"
#include "editor/game_layer.h"
#include "editor/editor_layer.h"
#include "render/render_layer.h"
#include "gui/gui_layer.h"

#include "render/pass/shadow_pass.h"
#include "render/pass/skybox_pass.h"
#include "render/pass/scene_pass.h"
#include "render/pass/editor_pass.h"
#include "render/pass/outline_pass.h"

#include "gui/sections/section_main_menu.h"
#include "gui/sections/section_scene_entities.h"
#include "gui/sections/section_scene_info.h"
#include "gui/sections/section_mesh_editor.h"
#include "gui/sections/section_texture_editor.h"
#include "gui/sections/section_material_editor.h"
#include "gui/sections/section_shader_editor.h"
#include "gui/sections/section_script_editor.h"
#include "gui/sections/section_viewport.h"
#include "gui/sections/section_scripting_parameters.h"
#include "gui/sections/section_log_view.h"
#include "gui/sections/section_explorer.h"
#include "gui/sections/section_entity_properties.h"
#include "gui/sections/section_time_info.h"


namespace titian {
    class TitianEditor : public LayerStack
    {
    public:
        kl::Object<AppLayer> app_layer = nullptr;
        kl::Object<GameLayer> game_layer = nullptr;
        kl::Object<EditorLayer> editor_layer = nullptr;
        kl::Object<RenderLayer> render_layer = nullptr;
        kl::Object<GUILayer> gui_layer = nullptr;

        TitianEditor();
        ~TitianEditor() override;
    };
}
