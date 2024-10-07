#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

#include "scripting/native_script.h"
#include "scripting/interp_script.h"
#include "scripting/node_script.h"

#include "gui/editors/code_editor.h"


namespace titian {
	struct GUISectionScriptEditor : GUISection
	{
		String selected_script = "/";

		GUISectionScriptEditor();

		void render_gui() override;

	private:
		MemoryEditor m_native_editor{};
		CodeEditor m_interp_editor{};
		Script* m_last_script = nullptr;

		void display_scripts(Scene& scene);
		void show_script_properties(Script* script) const;

		void edit_interp_script(InterpScript& script);
		void edit_node_script(NodeScript& script);
		void edit_native_script(NativeScript& script);
	};
}
