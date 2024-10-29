#include "titian.h"


titian::GUISectionScriptEditor::GUISectionScriptEditor()
	: GUISection("GUISectionScriptEditor")
{
	m_interp_editor.load_lua_standard();
}

void titian::GUISectionScriptEditor::render_gui()
{
	const TimeBomb _ = bench_time_bomb();

	kl::GPU& gpu = AppLayer::get().gpu;
	Scene& scene = GameLayer::get().scene();

	Ref<Script> script;
	if (scene.scripts.contains(selected_script)) {
		script = scene.scripts.at(selected_script);
	}

	NativeScript* native_script = &script.as<NativeScript>();
	InterpScript* interp_script = &script.as<InterpScript>();
	NodeScript* node_script = &script.as<NodeScript>();

	if (im::Begin("Script Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const float available_width = im::GetContentRegionAvail().x;
		im::Columns(2, "ScriptEditorColumns", false);

		im::SetColumnWidth(im::GetColumnIndex(), available_width * 0.25f);
		if (im::BeginChild("ScriptListChild")) {
			display_scripts(scene);
		}
		im::EndChild();
		im::NextColumn();

		if (native_script) {
			edit_native_script(*native_script);
		}
		else if (interp_script) {
			edit_interp_script(*interp_script);
		}
		else if (node_script) {
			edit_node_script(*node_script);
		}

		if (auto file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
			const fs::path path = file.value();
			const String extension = path.extension().string();
			if (extension == FILE_EXTENSION_LUA) {
				Ref script = new InterpScript();
				script->source = kl::read_file(path.string());
				script->reload();
				scene.scripts[Scene::generate_unique_name(path.filename().string(), scene.scripts)] = script;
			}
			else if (extension == FILE_EXTENSION_DLL) {
				Ref script = new NativeScript();
				script->data = kl::read_file(path.string());
				script->reload();
				scene.scripts[Scene::generate_unique_name(path.filename().string(), scene.scripts)] = script;
			}
		}

		show_script_properties(&script);
	}
	im::End();
}

void titian::GUISectionScriptEditor::display_scripts(Scene& scene)
{
	if (im::BeginPopupContextWindow("NewScript", ImGuiPopupFlags_MouseButtonMiddle)) {
		const String name = gui_input_continuous("##CreateScriptInput");
		if (!name.empty() && !scene.scripts.contains(name)) {
			if (im::MenuItem("New Interp Script")) {
				scene.scripts[name] = new InterpScript();
				im::CloseCurrentPopup();
			}
			if (im::MenuItem("New Node Script")) {
				scene.scripts[name] = new NodeScript();
				im::CloseCurrentPopup();
			}
			if (im::MenuItem("New Native Script")) {
				scene.scripts[name] = new NativeScript();
				im::CloseCurrentPopup();
			}
		}
		im::EndPopup();
	}

	const String filter = gui_input_continuous("Search###ScriptEditor");
	for (auto& [script_name, script] : scene.scripts) {
		if (!filter.empty() && script_name.find(filter) == -1) {
			continue;
		}

		if (im::Selectable(script_name.data(), script_name == selected_script)) {
			selected_script = script_name;
		}

		if (im::BeginPopupContextItem(script_name.data(), ImGuiPopupFlags_MouseButtonRight)) {
			bool should_break = false;
			im::Text("Edit Script");

			if (auto opt_name = gui_input_waited("##RenameScriptInput", script_name)) {
				const auto& name = opt_name.value();
				if (!name.empty() && !scene.scripts.contains(name)) {
					if (selected_script == script_name) {
						selected_script = name;
					}
					scene.scripts[name] = script;
					scene.scripts.erase(script_name);
					should_break = true;
					im::CloseCurrentPopup();
				}
			}

			if (im::Button("Reload", { -1.0f, 0.0f })) {
				script->reload();
				should_break = true;
				im::CloseCurrentPopup();
			}

			if (im::Button("Delete", { -1.0f, 0.0f })) {
				if (selected_script == script_name) {
					selected_script = "/";
				}
				scene.scripts.erase(script_name);
				should_break = true;
				im::CloseCurrentPopup();
			}

			im::EndPopup();
			if (should_break) {
				break;
			}
		}
	}
}

void titian::GUISectionScriptEditor::show_script_properties(Script* script) const
{
	GUILayer& gui_layer = GUILayer::get();

	if (im::Begin("Script Properties") && script) {
		im::Text("Info");

		im::Text("Name: ");
		im::SameLine();
		gui_colored_text(selected_script, gui_layer.special_color);

		im::Text("Type: ");
		im::SameLine();
		if (dynamic_cast<NativeScript*>(script)) {
			im::Text("NATIVE");
		}
		else if (dynamic_cast<InterpScript*>(script)) {
			im::Text("INTERP");
		}
		else if (dynamic_cast<NodeScript*>(script)) {
			im::Text("NODE");
		}
	}
	im::End();
}

void titian::GUISectionScriptEditor::edit_interp_script(InterpScript& script)
{
	if (&script != m_last_script) {
		m_last_script = &script;
		m_interp_editor.load(script.source);
	}
	im::PushFont(GUILayer::get().roboto_font_large);
	m_interp_editor.edit(script.source);
	im::PopFont();
}

void titian::GUISectionScriptEditor::edit_node_script(NodeScript& script)
{
	script.update_editor();
}

void titian::GUISectionScriptEditor::edit_native_script(NativeScript& script)
{
	auto& data = script.data;
	m_native_editor.DrawContents(data.data(), data.size());
}
