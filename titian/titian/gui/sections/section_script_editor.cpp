#include "main.h"


titian::GUISectionScriptEditor::GUISectionScriptEditor(EditorLayer* editor_layer, GUILayer* gui_layer)
	: GUISection("GUISectionScriptEditor")
	, editor_layer(editor_layer)
	, gui_layer(gui_layer)
	, m_native_editor()
	, m_interp_editor()
	, m_node_editor(nullptr)
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

	kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
	Scene* scene = &editor_layer->game_layer->scene;

	Script* script = &scene->get_script(selected_script);
	NativeScript* native_script = dynamic_cast<NativeScript*>(script);
	InterpScript* interp_script = dynamic_cast<InterpScript*>(script);
	NodeScript* node_script = dynamic_cast<NodeScript*>(script);

	if (ImGui::Begin("Script Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const float available_width = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(2, "ScriptEditorColumns", false);

		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
		if (ImGui::BeginChild("ScriptListChild")) {
			display_scripts(scene);
		}
		ImGui::EndChild();
		ImGui::NextColumn();

		if (native_script) {
			edit_native_script(native_script);
		}
		else if (interp_script) {
			edit_interp_script(interp_script);
		}
		else if (node_script) {
			edit_node_script(node_script);
		}
		show_script_properties(script);
	}
	ImGui::End();
}

void titian::GUISectionScriptEditor::display_scripts(Scene* scene)
{
	if (ImGui::BeginPopupContextWindow("NewScript", ImGuiPopupFlags_MouseButtonMiddle)) {
		const std::string name = gui_input_continuous("##CreateScriptInput");
		if (!name.empty()) {
			if (ImGui::MenuItem("New Interp Script") && !scene->scripts.contains(name)) {
				scene->scripts[name] = new InterpScript();
				ImGui::CloseCurrentPopup();
			}
#if 0
			if (ImGui::MenuItem("New Node Script") && !scene->scripts.contains(name)) {
				scene->scripts[name] = new NodeScript();
				ImGui::CloseCurrentPopup();
			}
#endif
			if (ImGui::MenuItem("New Native Script") && !scene->scripts.contains(name)) {
				scene->scripts[name] = new NativeScript();
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}

	const std::string filter = gui_input_continuous("Search###ScriptEditor");
	for (auto& [script_name, script] : scene->scripts) {
		if (!filter.empty() && script_name.find(filter) == -1) {
			continue;
		}

		// Script type
		if (script.is<const NativeScript>()) {
			ImGui::Button("NATIVE");
		}
		else if (script.is<const InterpScript>()) {
			ImGui::Button("INTERP");
		}
		else if (script.is<const NodeScript>()) {
			ImGui::Button("NODE");
		}
		else {
			ImGui::Button("SCRIPT");
		}
		ImGui::SameLine();

		// Script name
		if (ImGui::Selectable(script_name.c_str(), script_name == this->selected_script)) {
			this->selected_script = script_name;
		}

		if (ImGui::BeginPopupContextItem(script_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
			bool should_break = false;
			ImGui::Text("Edit Script");

			if (std::optional opt_name = gui_input_waited("##RenameScriptInput", script_name)) {
				const std::string& name = opt_name.value();
				if (!name.empty() && !scene->scripts.contains(name)) {
					if (this->selected_script == script_name) {
						this->selected_script = name;
					}
					scene->scripts[name] = script;
					scene->scripts.erase(script_name);
					should_break = true;
					ImGui::CloseCurrentPopup();
				}
			}

			if (ImGui::Button("Reload", { -1.0f, 0.0f })) {
				script->reload();
				should_break = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
				if (this->selected_script == script_name) {
					this->selected_script = "/";
				}
				scene->scripts.erase(script_name);
				should_break = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
			if (should_break) {
				break;
			}
		}
	}
}

void titian::GUISectionScriptEditor::show_script_properties(Script* script) const
{
	if (ImGui::Begin("Script Properties") && script) {
		ImGui::Text("Info");

		ImGui::Text("Name: ");
		ImGui::SameLine();
		gui_colored_text(selected_script, gui_layer->special_color);

		ImGui::Text("Type: ");
		ImGui::SameLine();
		if (dynamic_cast<NativeScript*>(script)) {
			ImGui::Text("NATIVE");
		}
		else if (dynamic_cast<InterpScript*>(script)) {
			ImGui::Text("INTERP");
		}
		else if (dynamic_cast<NodeScript*>(script)) {
			ImGui::Text("NODE");
		}
	}
	ImGui::End();
}

void titian::GUISectionScriptEditor::edit_native_script(NativeScript* script)
{
	std::vector<byte>& data = script->data;
	m_native_editor.DrawContents(data.data(), data.size());

	if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
		const std::filesystem::path path = file.value();
		const std::string extension = path.extension().string();
		if (extension == FILE_EXTENSION_NATIVE_SCRIPT) {
			script->data = kl::read_file(path.string());
			script->reload();
		}
	}
}

void titian::GUISectionScriptEditor::edit_interp_script(InterpScript* script)
{
	if (script != m_last_script) {
		m_last_script = script;
		m_interp_editor.load(script->source);
	}

	ImGui::PushFont(gui_layer->roboto_font_large);
	m_interp_editor.edit(&script->source);

	if (ImGui::Begin("Code Suggestion", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const TextEditor::LanguageDefinition* definition = m_interp_editor.get_definition();
		const std::string current_word = m_interp_editor.get_word_at_cursor();
		if (definition && !current_word.empty()) {
			const TextEditor::Palette& palette = *m_interp_editor.get_palette();
			ImGui::PushStyleColor(ImGuiCol_Text, (ImU32) ImColor(220, 206, 125, 255));
			for (const auto& [member, _] : INTERP_SCRIPT_MEMBERS) {
				if (member.find(current_word) != -1) {
					if (ImGui::MenuItem(member.c_str())) {
						m_interp_editor.replace_word_at_cursor(member);
					}
				}
			}
			ImGui::PushStyleColor(ImGuiCol_Text, palette[1]);
			for (const auto& keyword : definition->mKeywords) {
				if (keyword.find(current_word) != -1) {
					if (ImGui::MenuItem(keyword.c_str())) {
						m_interp_editor.replace_word_at_cursor(keyword);
					}
				}
			}
			ImGui::PushStyleColor(ImGuiCol_Text, palette[8]);
			for (const auto& [identifier, _] : definition->mIdentifiers) {
				if (identifier.find(current_word) != -1) {
					if (ImGui::MenuItem(identifier.c_str())) {
						m_interp_editor.replace_word_at_cursor(identifier);
					}
				}
			}
			ImGui::PopStyleColor(3);
		}
	}
	ImGui::End();
	ImGui::PopFont();

	if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
		const std::filesystem::path path = file.value();
		const std::string extension = path.extension().string();
		if (extension == FILE_EXTENSION_INTERP_SCRIPT) {
			script->source = kl::read_file_string(path.string());
			script->reload();
			m_last_script = nullptr;
		}
	}
}

void titian::GUISectionScriptEditor::edit_node_script(NodeScript* script)
{
	ImGui::Text("Node Editor");
	ImGui::Separator();

	ed::SetCurrentEditor(m_node_editor);
	ed::Begin("Node Editor", ImVec2(-1.0f, -1.0f));

	auto display_node = [&](const Node& node)
	{
		auto display_pin = [&](const Pin& pin, PinKind kind)
		{
			ed::BeginPin(pin.id, kind);
			ImGui::Text(pin.title.c_str());
			ed::EndPin();
		};

		ed::BeginNode(node.id);
		ImGui::Text(node.title.c_str());

		ImGui::BeginGroup();
		if (node.flow_input) {
			display_pin(node.flow_input.value(), PinKind::Input);
		}
		for (auto& pin : node.input_pins) {
			display_pin(pin, PinKind::Input);
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		if (node.flow_output) {
			display_pin(node.flow_output.value(), PinKind::Output);
		}
		for (auto& pin : node.output_pins) {
			display_pin(pin, PinKind::Output);
		}
		ImGui::EndGroup();

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
