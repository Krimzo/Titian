#include "main.h"


titian::GUISectionScriptEditor::GUISectionScriptEditor(EditorLayer* editor_layer, GUILayer* gui_layer)
{
	this->editor_layer = editor_layer;
	this->gui_layer = gui_layer;

	// Text editor
	m_text_editor.SetTabSize(4);
	m_text_editor.SetShowWhitespaces(false);
	m_text_editor.SetColorizerEnable(true);

	TextEditor::LanguageDefinition language_definition = m_text_editor.GetLanguageDefinition();
	language_definition.mName = "Chaiscript";
	language_definition.mAutoIndentation = true;
	language_definition.mCaseSensitive = true;
	language_definition.mCommentStart = "/*";
	language_definition.mCommentEnd = "*/";
	language_definition.mSingleLineComment = "/";
	language_definition.mKeywords = INTER_SCRIPT_KEYWORDS;
	language_definition.mIdentifiers.clear();
	for (const auto& [name, info] : INTER_SCRIPT_IDENTIFIERS) {
		TextEditor::Identifier identifier = {};
		identifier.mDeclaration = info;
		language_definition.mIdentifiers[name] = identifier;
	}
	m_text_editor.SetLanguageDefinition(language_definition);

	TextEditor::Palette color_pallete = m_text_editor.GetDarkPalette();
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Keyword)] = ImColor(240, 155, 120);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Number)] = ImColor(200, 95, 95);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::String)] = ImColor(211, 158, 104);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::CharLiteral)] = ImColor(204, 170, 135);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Punctuation)] = ImColor(225, 225, 225);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Preprocessor)] = ImColor(224, 179, 215);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Identifier)] = ImColor(210, 210, 210);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::KnownIdentifier)] = ImColor(105, 210, 190);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::PreprocIdentifier)] = ImColor(210, 178, 203);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Comment)] = ImColor(128, 128, 128);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::MultiLineComment)] = ImColor(116, 116, 116);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Background)] = ImColor(25, 25, 25);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Cursor)] = ImColor(220, 220, 220);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Selection)] = ImColor(70, 70, 70);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::ErrorMarker)] = ImColor(196, 57, 57);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::Breakpoint)] = ImColor(222, 73, 73);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::LineNumber)] = ImColor(210, 210, 210);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::CurrentLineFill)] = ImColor(51, 51, 51, 10);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::CurrentLineFillInactive)] = ImColor(51, 51, 51, 20);
	color_pallete[static_cast<ImU32>(TextEditor::PaletteIndex::CurrentLineEdge)] = ImColor(89, 89, 89);
	m_text_editor.SetPalette(color_pallete);
}

void titian::GUISectionScriptEditor::render_gui()
{
	kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
	Scene* scene = &editor_layer->game_layer->scene;

	if (ImGui::Begin("Script Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const float available_width = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(2, "ScriptEditorColumns", false);

		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
		if (ImGui::BeginChild("ScriptListChild")) {
			// Create script
			if (ImGui::BeginPopupContextWindow("NewScript", ImGuiPopupFlags_MouseButtonMiddle)) {
				const std::string name = gui_input_continuous("##CreateScriptInput");
				if (!name.empty()) {
					if (ImGui::MenuItem("New Native Script") && !scene->scripts.contains(name)) {
						scene->scripts[name] = new NativeScript();
						ImGui::CloseCurrentPopup();
					}
					if (ImGui::MenuItem("New Inter Script") && !scene->scripts.contains(name)) {
						scene->scripts[name] = new InterScript();
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}

			// Scripts
			display_scripts(scene);
		}
		ImGui::EndChild();

		if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
			const std::filesystem::path path = file.value();
			const std::string extension = path.extension().string();
			if (extension == FILE_EXTENSION_NATIVE_SCRIPT) {
				kl::Object new_script = new NativeScript();
				new_script->path = path.string();
				new_script->reload();
				scene->scripts[path.stem().string()] = new_script;
			}
			else if (extension == FILE_EXTENSION_INTER_SCRIPT) {
				kl::Object new_script = new InterScript();
				new_script->path = path.string();
				new_script->reload();
				scene->scripts[path.stem().string()] = new_script;
			}
		}

		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

		Script* script = &scene->get_script(selected_script);
		NativeScript* native_script = dynamic_cast<NativeScript*>(script);
		InterScript* inter_script = dynamic_cast<InterScript*>(script);

		if (ImGui::BeginChild("ScriptEditorChild")) {
			if (native_script) {
				edit_native_script(native_script);
			}
			else if (inter_script) {
				edit_inter_script(inter_script);
			}
		}
		ImGui::EndChild();

		ImGui::PopStyleVar(2);

		show_script_properties(script);
	}
	ImGui::End();
}

void titian::GUISectionScriptEditor::display_scripts(Scene* scene)
{
	const std::string filter = gui_input_continuous("Search###ScriptEditor");
	for (auto& [script_name, script] : scene->scripts) {
		if (!filter.empty() && script_name.find(filter) == -1) {
			continue;
		}

		// Script type
		if (dynamic_cast<const NativeScript*>(&script)) {
			ImGui::Button("NATIVE");
		}
		else if (dynamic_cast<const InterScript*>(&script)) {
			ImGui::Button("INTER");
		}
		else {
			ImGui::Button("SCRIPT");
		}
		ImGui::SameLine();

		// Script name
		if (ImGui::Selectable(script_name.c_str(), script_name == this->selected_script)) {
			this->selected_script = script_name;

			if (const InterScript* inter_script = dynamic_cast<const InterScript*>(&script)) {
				m_text_editor.SetText(inter_script->source);
			}
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
				if (InterScript* inter_script = dynamic_cast<InterScript*>(&script)) {
					m_text_editor.SetText(inter_script->source);
				}
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

void titian::GUISectionScriptEditor::edit_native_script(NativeScript* script)
{
	std::vector<byte>& data = script->data;
	m_memory_editor.DrawContents(data.data(), data.size());
}

void titian::GUISectionScriptEditor::edit_inter_script(InterScript* script)
{
	ImGui::PushFont(gui_layer->jetbrains_font);

	//m_text_editor.SetText(script->source);
	m_text_editor.Render(selected_script.c_str());
	script->source = m_text_editor.GetText();

	ImGui::PopFont();
}

void titian::GUISectionScriptEditor::show_script_properties(Script* script)
{
	if (ImGui::Begin("Script Properties") && script) {
		ImGui::Text("Info");

		ImGui::Text("Name: ");
		ImGui::SameLine();
		gui_colored_text(selected_script, gui_layer->special_color);

		ImGui::Text("Type: ");
		ImGui::SameLine();
		if (NativeScript* native_script = dynamic_cast<NativeScript*>(script)) {
			ImGui::Text("NATIVE");
		}
		else if (InterScript* inter_script = dynamic_cast<InterScript*>(script)) {
			ImGui::Text("INTER");
		}

		std::string buffer = script->path;
		buffer.resize(150);
		if (ImGui::InputText("Path", buffer.data(), buffer.size())) {
			script->path.clear();
			for (const char value : buffer) {
				if (!value) {
					break;
				}
				script->path.push_back(value);
			}
		}
	}
	ImGui::End();
}
