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

	if (imgui::Begin("Script Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const float available_width = imgui::GetContentRegionAvail().x;
		imgui::Columns(2, "ScriptEditorColumns", false);

		imgui::SetColumnWidth(imgui::GetColumnIndex(), available_width * 0.25f);
		if (imgui::BeginChild("ScriptListChild")) {
			display_scripts(scene);
		}
		imgui::EndChild();
		imgui::NextColumn();

		if (native_script) {
			edit_native_script(native_script);
		}
		else if (interp_script) {
			edit_interp_script(interp_script);
		}
		else if (node_script) {
			edit_node_script(node_script);
		}

		if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
			const std::filesystem::path path = file.value();
			const std::string extension = path.extension().string();
			if (extension == FILE_EXTENSION_CHAI) {
				kl::Object script = new InterpScript();
				script->source = kl::read_file_string(path.string());
				script->reload();
				scene->scripts[scene->generate_unique_name(path.filename().string(), scene->scripts)] = script;
			}
			else if (extension == FILE_EXTENSION_DLL) {
				kl::Object script = new NativeScript();
				script->data = kl::read_file(path.string());
				script->reload();
				scene->scripts[scene->generate_unique_name(path.filename().string(), scene->scripts)] = script;
			}
		}

		show_script_properties(script);
	}
	imgui::End();
}

void titian::GUISectionScriptEditor::display_scripts(Scene* scene)
{
	if (imgui::BeginPopupContextWindow("NewScript", ImGuiPopupFlags_MouseButtonMiddle)) {
		const std::string name = gui_input_continuous("##CreateScriptInput");
		if (!name.empty() && !scene->scripts.contains(name)) {
			if (imgui::MenuItem("New Interp Script")) {
				scene->scripts[name] = new InterpScript();
				imgui::CloseCurrentPopup();
			}
#if false
			if (imgui::MenuItem("New Node Script")) {
				scene->scripts[name] = new NodeScript();
				imgui::CloseCurrentPopup();
			}
#endif
			if (imgui::MenuItem("New Native Script")) {
				scene->scripts[name] = new NativeScript();
				imgui::CloseCurrentPopup();
			}
		}
		imgui::EndPopup();
	}

	const std::string filter = gui_input_continuous("Search###ScriptEditor");
	for (auto& [script_name, script] : scene->scripts) {
		if (!filter.empty() && script_name.find(filter) == -1) {
			continue;
		}

		// Script type
		if (script.is<const NativeScript>()) {
			imgui::Button("NATIVE");
		}
		else if (script.is<const InterpScript>()) {
			imgui::Button("INTERP");
		}
		else if (script.is<const NodeScript>()) {
			imgui::Button("NODE");
		}
		else {
			imgui::Button("SCRIPT");
		}
		imgui::SameLine();

		// Script name
		if (imgui::Selectable(script_name.c_str(), script_name == this->selected_script)) {
			this->selected_script = script_name;
		}

		if (imgui::BeginPopupContextItem(script_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
			bool should_break = false;
			imgui::Text("Edit Script");

			if (std::optional opt_name = gui_input_waited("##RenameScriptInput", script_name)) {
				const std::string& name = opt_name.value();
				if (!name.empty() && !scene->scripts.contains(name)) {
					if (this->selected_script == script_name) {
						this->selected_script = name;
					}
					scene->scripts[name] = script;
					scene->scripts.erase(script_name);
					should_break = true;
					imgui::CloseCurrentPopup();
				}
			}

			if (imgui::Button("Reload", { -1.0f, 0.0f })) {
				script->reload();
				should_break = true;
				imgui::CloseCurrentPopup();
			}

			if (imgui::Button("Delete", { -1.0f, 0.0f })) {
				if (this->selected_script == script_name) {
					this->selected_script = "/";
				}
				scene->scripts.erase(script_name);
				should_break = true;
				imgui::CloseCurrentPopup();
			}

			imgui::EndPopup();
			if (should_break) {
				break;
			}
		}
	}
}

void titian::GUISectionScriptEditor::show_script_properties(Script* script) const
{
	if (imgui::Begin("Script Properties") && script) {
		imgui::Text("Info");

		imgui::Text("Name: ");
		imgui::SameLine();
		gui_colored_text(selected_script, gui_layer->special_color);

		imgui::Text("Type: ");
		imgui::SameLine();
		if (dynamic_cast<NativeScript*>(script)) {
			imgui::Text("NATIVE");
		}
		else if (dynamic_cast<InterpScript*>(script)) {
			imgui::Text("INTERP");
		}
		else if (dynamic_cast<NodeScript*>(script)) {
			imgui::Text("NODE");
		}
	}
	imgui::End();
}

void titian::GUISectionScriptEditor::edit_native_script(NativeScript* script)
{
	std::vector<byte>& data = script->data;
	m_native_editor.DrawContents(data.data(), data.size());
}

void titian::GUISectionScriptEditor::edit_interp_script(InterpScript* script)
{
	if (script != m_last_script) {
		m_last_script = script;
		m_interp_editor.load(script->source);
	}

	imgui::PushFont(gui_layer->roboto_font_large);
	m_interp_editor.edit(&script->source);

	if (imgui::Begin("Code Suggestion", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const TextEditor::LanguageDefinition* definition = m_interp_editor.get_definition();
		const std::string current_word = m_interp_editor.get_word_at_cursor();
		if (definition && !current_word.empty()) {
			const TextEditor::Palette& palette = *m_interp_editor.get_palette();
			imgui::PushStyleColor(ImGuiCol_Text, (ImU32) ImColor(220, 206, 125, 255));
			for (const auto& [member, _] : INTERP_SCRIPT_MEMBERS) {
				if (member.find(current_word) != -1) {
					if (imgui::MenuItem(member.c_str())) {
						m_interp_editor.replace_word_at_cursor(member);
					}
				}
			}
			imgui::PushStyleColor(ImGuiCol_Text, palette[1]);
			for (const auto& keyword : definition->mKeywords) {
				if (keyword.find(current_word) != -1) {
					if (imgui::MenuItem(keyword.c_str())) {
						m_interp_editor.replace_word_at_cursor(keyword);
					}
				}
			}
			imgui::PushStyleColor(ImGuiCol_Text, palette[8]);
			for (const auto& [identifier, _] : definition->mIdentifiers) {
				if (identifier.find(current_word) != -1) {
					if (imgui::MenuItem(identifier.c_str())) {
						m_interp_editor.replace_word_at_cursor(identifier);
					}
				}
			}
			imgui::PopStyleColor(3);
		}
	}
	imgui::End();
	imgui::PopFont();
}

void titian::GUISectionScriptEditor::edit_node_script(NodeScript* script)
{
	imgui::Text("Node Editor");
	imgui::Separator();

	ed::SetCurrentEditor(m_node_editor);
	ed::Begin("Node Editor", ImVec2(-1.0f, -1.0f));

	auto display_node = [&](const Node& node)
	{
		auto display_pin = [&](const Pin& pin, PinKind kind)
		{
			ed::BeginPin(pin.id, kind);
			imgui::Text(pin.title.c_str());
			ed::EndPin();
		};

		ed::BeginNode(node.id);
		imgui::Text(node.title.c_str());

		imgui::BeginGroup();
		if (node.flow_input) {
			display_pin(node.flow_input.value(), PinKind::Input);
		}
		for (auto& pin : node.input_pins) {
			display_pin(pin, PinKind::Input);
		}
		imgui::EndGroup();
		imgui::SameLine();

		imgui::BeginGroup();
		if (node.flow_output) {
			display_pin(node.flow_output.value(), PinKind::Output);
		}
		for (auto& pin : node.output_pins) {
			display_pin(pin, PinKind::Output);
		}
		imgui::EndGroup();

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
