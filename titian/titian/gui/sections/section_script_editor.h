#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

#include "scripting/native_script.h"
#include "scripting/inter_script.h"


namespace titian {
	class GUISectionScriptEditor : public GUISection
	{
	public:
		EditorLayer* editor_layer = nullptr;
		GUILayer* gui_layer = nullptr;

		std::string selected_script = "/";

		GUISectionScriptEditor(EditorLayer* editor_layer, GUILayer* gui_layer);

		void render_gui() override;

	private:
		MemoryEditor m_memory_editor = {};

		void display_scripts(Scene* scene);
		void show_script_properties(Script* script) const;

		void edit_native_script(NativeScript* script);
		void edit_inter_script(InterScript* script);
	};
}
