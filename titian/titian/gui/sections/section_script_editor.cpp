#include "main.h"


titian::GUISectionScriptEditor::GUISectionScriptEditor(const LayerPackage& package)
	: GUISection("GUISectionScriptEditor", package)
{
	m_interp_editor.load_chai_standard();

	ed::Config node_config{};
	node_config.SettingsFile = "node_editor.json";
	m_node_editor = ed::CreateEditor(&node_config);
}

titian::GUISectionScriptEditor::~GUISectionScriptEditor()
{
	ed::DestroyEditor(m_node_editor);
}

void titian::GUISectionScriptEditor::render_gui()
{
	const TimeBomb _ = this->time_it();

	kl::GPU* gpu = &app_layer->gpu;
	Scene* scene = &game_layer->scene;

	Script* script = &scene->get_script(selected_script);
	NativeScript* native_script = dynamic_cast<NativeScript*>(script);
	InterpScript* interp_script = dynamic_cast<InterpScript*>(script);
	NodeScript* node_script = dynamic_cast<NodeScript*>(script);

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
			edit_native_script(native_script);
		}
		else if (interp_script) {
			edit_interp_script(interp_script);
		}
		else if (node_script) {
			edit_node_script(node_script);
		}

		if (const Optional file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
			const fs::path path = file.value();
			const String extension = path.extension().string();
			if (extension == FILE_EXTENSION_CHAI) {
				Ref script = new InterpScript();
				script->source = kl::read_file_string(path.string());
				script->reload();
				scene->scripts[scene->generate_unique_name(path.filename().string(), scene->scripts)] = script;
			}
			else if (extension == FILE_EXTENSION_DLL) {
				Ref script = new NativeScript();
				script->data = kl::read_file(path.string());
				script->reload();
				scene->scripts[scene->generate_unique_name(path.filename().string(), scene->scripts)] = script;
			}
		}

		show_script_properties(script);
	}
	im::End();
}

void titian::GUISectionScriptEditor::display_scripts(Scene* scene)
{
	if (im::BeginPopupContextWindow("NewScript", ImGuiPopupFlags_MouseButtonMiddle)) {
		const String name = gui_input_continuous("##CreateScriptInput");
		if (!name.empty() && !scene->scripts.contains(name)) {
			if (im::MenuItem("New Interp Script")) {
				scene->scripts[name] = new InterpScript();
				im::CloseCurrentPopup();
			}
#if false
			if (im::MenuItem("New Node Script")) {
				scene->scripts[name] = new NodeScript();
				im::CloseCurrentPopup();
			}
#endif
			if (im::MenuItem("New Native Script")) {
				scene->scripts[name] = new NativeScript();
				im::CloseCurrentPopup();
			}
		}
		im::EndPopup();
	}

	const String filter = gui_input_continuous("Search###ScriptEditor");
	for (auto& [script_name, script] : scene->scripts) {
		if (!filter.empty() && script_name.find(filter) == -1) {
			continue;
		}

		// Script type
		if (script.is<const NativeScript>()) {
			im::Button("NATIVE");
		}
		else if (script.is<const InterpScript>()) {
			im::Button("INTERP");
		}
		else if (script.is<const NodeScript>()) {
			im::Button("NODE");
		}
		else {
			im::Button("SCRIPT");
		}
		im::SameLine();

		// Script name
		if (im::Selectable(script_name.c_str(), script_name == this->selected_script)) {
			this->selected_script = script_name;
		}

		if (im::BeginPopupContextItem(script_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
			bool should_break = false;
			im::Text("Edit Script");

			if (Optional opt_name = gui_input_waited("##RenameScriptInput", script_name)) {
				const String& name = opt_name.value();
				if (!name.empty() && !scene->scripts.contains(name)) {
					if (this->selected_script == script_name) {
						this->selected_script = name;
					}
					scene->scripts[name] = script;
					scene->scripts.erase(script_name);
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
				if (this->selected_script == script_name) {
					this->selected_script = "/";
				}
				scene->scripts.erase(script_name);
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
	if (im::Begin("Script Properties") && script) {
		im::Text("Info");

		im::Text("Name: ");
		im::SameLine();
		gui_colored_text(selected_script, gui_layer->special_color);

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

void titian::GUISectionScriptEditor::edit_native_script(NativeScript* script)
{
	Vector<byte>& data = script->data;
	m_native_editor.DrawContents(data.data(), data.size());
}

void titian::GUISectionScriptEditor::edit_interp_script(InterpScript* script)
{
	if (script != m_last_script) {
		m_last_script = script;
		m_interp_editor.load(script->source);
	}

	im::PushFont(gui_layer->roboto_font_large);
	m_interp_editor.edit(&script->source);

	if (im::Begin("Code Suggestion", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const TextEditor::LanguageDefinition* definition = m_interp_editor.get_definition();
		const String current_word = m_interp_editor.get_word_at_cursor();
		if (definition && !current_word.empty()) {
			const TextEditor::Palette& palette = *m_interp_editor.get_palette();
			im::PushStyleColor(ImGuiCol_Text, (ImU32) ImColor(220, 206, 125, 255));
			for (const auto& [member, _] : INTERP_SCRIPT_MEMBERS) {
				if (member.find(current_word) != -1) {
					if (im::MenuItem(member.c_str())) {
						m_interp_editor.replace_word_at_cursor(member);
					}
				}
			}
			im::PushStyleColor(ImGuiCol_Text, palette[1]);
			for (const auto& keyword : definition->mKeywords) {
				if (keyword.find(current_word) != -1) {
					if (im::MenuItem(keyword.c_str())) {
						m_interp_editor.replace_word_at_cursor(keyword);
					}
				}
			}
			im::PushStyleColor(ImGuiCol_Text, palette[8]);
			for (const auto& [identifier, _] : definition->mIdentifiers) {
				if (identifier.find(current_word) != -1) {
					if (im::MenuItem(identifier.c_str())) {
						m_interp_editor.replace_word_at_cursor(identifier);
					}
				}
			}
			im::PopStyleColor(3);
		}
	}
	im::End();
	im::PopFont();
}

void titian::GUISectionScriptEditor::edit_node_script(NodeScript* script)
{
	im::Text("Node Editor");
	im::Separator();

	ed::SetCurrentEditor(m_node_editor);
	ed::Begin("Node Editor", ImVec2(-1.0f, -1.0f));

	auto display_node = [&](const Node& node)
	{
		auto display_pin = [&](const Pin& pin, PinKind kind)
		{
			ed::BeginPin(pin.id, kind);
			im::Text(pin.title.c_str());
			ed::EndPin();
		};

		ed::BeginNode(node.id);
		im::Text(node.title.c_str());

		im::BeginGroup();
		if (node.flow_input) {
			display_pin(node.flow_input.value(), PinKind::Input);
		}
		for (auto& pin : node.input_pins) {
			display_pin(pin, PinKind::Input);
		}
		im::EndGroup();
		im::SameLine();

		im::BeginGroup();
		if (node.flow_output) {
			display_pin(node.flow_output.value(), PinKind::Output);
		}
		for (auto& pin : node.output_pins) {
			display_pin(pin, PinKind::Output);
		}
		im::EndGroup();

		ed::EndNode();
	};

	/* WIP, maybe in the future */

	//display_node(script->start_node);
	//display_node(script->update_node);
	//for (auto& node : script->nodes) {
	//	display_node(node);
	//}

	ed::End();
	ed::SetCurrentEditor(nullptr);
}
