#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

#include "scripting/native_script.h"
#include "scripting/interp_script.h"
#include "scripting/node_script.h"

namespace ed = ax::NodeEditor;

namespace titian {
	class GUISectionScriptEditor : public GUISection
	{
	public:
		EditorLayer* editor_layer = nullptr;
		GUILayer* gui_layer = nullptr;

		std::string selected_script = "/";

		GUISectionScriptEditor(EditorLayer* editor_layer, GUILayer* gui_layer);
		~GUISectionScriptEditor() override;

		void render_gui() override;

	private:
		MemoryEditor m_memory_editor = {};
		ed::EditorContext* m_node_editor = nullptr;

		void display_scripts(Scene* scene);
		void show_script_properties(Script* script) const;

		void edit_native_script(NativeScript* script);
		void edit_interp_script(InterpScript* script);
		void edit_node_script(NodeScript* script);
	};
}
