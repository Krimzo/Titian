#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

#include "scripting/native_script.h"
#include "scripting/interp_script.h"
#include "scripting/node_script.h"

#include "gui/editors/code_editor.h"


namespace ed = ax::NodeEditor;

namespace titian {
	class GUISectionScriptEditor : public GUISection
	{
	public:
		String selected_script = "/";

		GUISectionScriptEditor(const LayerPackage& package);
		~GUISectionScriptEditor() override;

		void render_gui() override;

	private:
		MemoryEditor m_native_editor{};
		CodeEditor m_interp_editor{};
		ed::EditorContext* m_node_editor = nullptr;
		Script* m_last_script = nullptr;

		void display_scripts(Scene* scene);
		void show_script_properties(Script* script) const;

		void edit_native_script(NativeScript* script);
		void edit_interp_script(InterpScript* script);
		void edit_node_script(NodeScript* script);
	};
}
