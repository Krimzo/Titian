export module section_script_editor;

export import gui_section;
export import editor_layer;
export import gui_layer;

export import native_script;
export import interpreted_script;
export import node_script;

export namespace titian {
	class GUISectionScriptEditor : public GUISection
	{
	public:
		kl::Object<EditorLayer> editor_layer = nullptr;
		kl::Object<GUILayer> gui_layer = nullptr;

		std::string selected_script = "/";

		GUISectionScriptEditor(kl::Object<EditorLayer>& editor_layer, kl::Object<GUILayer>& gui_layer)
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
			language_definition.mKeywords = INTERPRETED_SCRIPT_KEYWORDS;
			language_definition.mIdentifiers.clear();
			for (const auto& [name, info] : INTERPRETED_SCRIPT_IDENTIFIERS) {
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

			// Node editor
			auto& style = ImNodes::GetStyle();
			style.Colors[ImNodesCol_TitleBar] = ImColor(255, 255, 255, 255);
			style.Colors[ImNodesCol_TitleBarHovered] = style.Colors[ImNodesCol_TitleBar];
			style.Colors[ImNodesCol_TitleBarSelected] = style.Colors[ImNodesCol_TitleBar];

			style.Colors[ImNodesCol_Link] = ImColor(255, 255, 255, 255);
			style.Colors[ImNodesCol_LinkHovered] = style.Colors[ImNodesCol_Link];
			style.Colors[ImNodesCol_LinkSelected] = style.Colors[ImNodesCol_Link];

			style.Colors[ImNodesCol_GridBackground] = ImColor(30, 30, 30, 255);
			style.Colors[ImNodesCol_BoxSelectorOutline] = ImColor(255, 255, 255, 255);
			style.Colors[ImNodesCol_BoxSelector] = ImColor(255, 255, 255, 10);
		}

		~GUISectionScriptEditor() override
		{}

		void render_gui() override
		{
			kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
			Scene* scene = &editor_layer->game_layer->scene;

			if (ImGui::Begin("Script Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
				const float available_width = ImGui::GetContentRegionAvail().x;
				ImGui::Columns(2, "ScriptEditorColumns", false);

				ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
				if (ImGui::BeginChild("ScriptListChild")) {
					if (ImGui::BeginPopupContextWindow()) {
						if (ImGui::Button("New NATIVE Script")) {
							std::string possible_name = "new_script";
							for (int i = 0; scene->scripts.contains(possible_name); i++) {
								possible_name = kl::format("new_script", i);
							}
							scene->scripts[possible_name] = new NativeScript();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("New INTERPRETED Script")) {
							std::string possible_name = "new_script";
							for (int i = 0; scene->scripts.contains(possible_name); i++) {
								possible_name = kl::format("new_script", i);
							}
							scene->scripts[possible_name] = new InterpretedScript();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("New NODE Script")) {
							std::string possible_name = "new_script";
							for (int i = 0; scene->scripts.contains(possible_name); i++) {
								possible_name = kl::format("new_script", i);
							}
							scene->scripts[possible_name] = new NodeScript();
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}

					display_scripts(scene);
				}
				ImGui::EndChild();

				const std::optional script_path = gui_get_drag_drop<std::string>("ScriptFile");
				if (script_path) {
					const std::filesystem::path path{ script_path.value() };
					const std::string extension = path.extension().string();
					if (extension == ".chai") {
						kl::Object new_script = new InterpretedScript();
						new_script->path = path.string();
						new_script->reload();
						scene->scripts[path.stem().string()] = new_script;
					}
					else if (extension == ".dll") {

					}
				}

				ImGui::NextColumn();

				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

				Script* script = &scene->get_script(selected_script);
				NativeScript* native_script = dynamic_cast<NativeScript*>(script);
				InterpretedScript* interpreted_script = dynamic_cast<InterpretedScript*>(script);
				NodeScript* node_script = dynamic_cast<NodeScript*>(script);

				if (ImGui::BeginChild("ScriptEditorChild")) {
					if (native_script) {
						edit_native_script(native_script);
					}
					else if (interpreted_script) {
						edit_interpreted_script(interpreted_script);
					}
					else if (node_script) {
						edit_node_script(node_script);
					}
				}
				ImGui::EndChild();

				ImGui::PopStyleVar(2);
			}
			ImGui::End();
		}

	private:
		MemoryEditor m_memory_editor = {};
		TextEditor m_text_editor = {};
		bool m_should_open_node_popup = false;

		void display_scripts(Scene* scene)
		{
			const std::string filter = gui_input_continuous("Search###ScriptEditor");
			for (const auto& [script_name, script] : scene->scripts) {
				if (!filter.empty() && !script_name.contains(filter)) {
					continue;
				}

				if (dynamic_cast<const NativeScript*>(&script)) {
					ImGui::Button("NATIVE");
					ImGui::SameLine();
				}
				else if (dynamic_cast<const InterpretedScript*>(&script)) {
					ImGui::Button("INTER");
					ImGui::SameLine();
				}
				else if (dynamic_cast<const NodeScript*>(&script)) {
					ImGui::Button("NODE");
					ImGui::SameLine();
				}

				if (ImGui::Selectable(script_name.c_str(), script_name == this->selected_script)) {
					this->selected_script = script_name;

					if (const InterpretedScript* inter_script = dynamic_cast<const InterpretedScript*>(&script)) {
						m_text_editor.SetText(inter_script->source);
					}
				}

				if (ImGui::BeginPopupContextItem(script_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
					bool should_break = false;
					ImGui::Text("Edit Script");

					if (std::optional name = gui_input_waited("##RenameScriptInput", script_name)) {
						if (!scene->scripts.contains(name.value())) {
							if (this->selected_script == script_name) {
								this->selected_script = name.value();
							}
							scene->scripts[name.value()] = script;
							scene->scripts.erase(script_name);
							should_break = true;
							ImGui::CloseCurrentPopup();
						}
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

		void edit_native_script(NativeScript* script)
		{
			std::vector<byte>& data = script->data;
			m_memory_editor.DrawContents(data.data(), data.size());
		}

		void edit_interpreted_script(InterpretedScript* script)
		{
			ImGui::PushFont(gui_layer->jetbrains_font);

			//m_text_editor.SetText(script->source);
			m_text_editor.Render(selected_script.c_str());
			script->source = m_text_editor.GetText();

			ImGui::PopFont();
		}

		void edit_node_script(NodeScript* script)
		{
			ImNodes::BeginNodeEditor();

			// Add nodes
			if (ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				m_should_open_node_popup = true;
			}
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				m_should_open_node_popup = false;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 5.0f, 5.0f });

			if (m_should_open_node_popup) {
				ImGui::OpenPopup("add node");
			}
			if (ImGui::BeginPopup("add node")) {
				const ImVec2 mouse_position = ImGui::GetMousePosOnOpeningCurrentPopup();
				const std::string filter = gui_input_continuous("Search###NodeScript");
				for (const auto& [node_name, node_function] : NODE_TYPES) {
					if (!filter.empty() && !node_name.contains(filter)) {
						continue;
					}
					if (ImGui::MenuItem(node_name.c_str())) {
						kl::Object node = node_function();
						ImNodes::SetNodeScreenSpacePos(node->id, mouse_position);
						script->nodes.push_back(node);
					}
				}
				ImGui::EndPopup();
			}

			ImGui::PopStyleVar();

			// Render
			for (auto& node : script->nodes) {
				ImNodes::BeginNode(node->id);

				ImNodes::BeginNodeTitleBar();
				gui_colored_text(node->title, kl::Color(30, 30, 30));
				ImNodes::EndNodeTitleBar();

				for (auto& pin : node->pins) {
					const uint32_t pin_color = pin->color();
					ImNodes::PushColorStyle(ImNodesCol_Pin, pin_color);
					ImNodes::PushColorStyle(ImNodesCol_PinHovered, pin_color);

					auto begin_func = pin->kind == FlowNodePin::Kind::INPUT ? ImNodes::BeginInputAttribute : ImNodes::BeginOutputAttribute;
					begin_func(pin->id, pin->shape());

					ImGui::Text(pin->name.c_str());

					auto end_func = pin->kind == FlowNodePin::Kind::INPUT ? ImNodes::EndInputAttribute : ImNodes::EndOutputAttribute;
					end_func();

					for (int i = 0; i < 2; i++) {
						ImNodes::PopColorStyle();
					}
				}

				ImNodes::EndNode();
			}

			ImNodes::MiniMap();
			ImNodes::EndNodeEditor();
		}
	};
}
