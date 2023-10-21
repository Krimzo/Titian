export module section_script_editor;

export import gui_section;
export import editor_layer;

export import native_script;
export import interpreted_script;
export import node_script;

export namespace titian {
	class GUISectionScriptEditor : public GUISection
	{
	public:
		kl::Object<EditorLayer> editor_layer = nullptr;

		std::string selected_script = "/";

		GUISectionScriptEditor(kl::Object<EditorLayer>& editor_layer)
		{
			this->editor_layer = editor_layer;

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
							scene->scripts[possible_name] = new NativeScript(scene);
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("New INTERPRETED Script")) {
							std::string possible_name = "new_script";
							for (int i = 0; scene->scripts.contains(possible_name); i++) {
								possible_name = kl::format("new_script", i);
							}
							scene->scripts[possible_name] = new InterpretedScript(scene);
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("New NODE Script")) {
							std::string possible_name = "new_script";
							for (int i = 0; scene->scripts.contains(possible_name); i++) {
								possible_name = kl::format("new_script", i);
							}
							scene->scripts[possible_name] = new NodeScript(scene);
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}

					display_scripts(scene);
				}
				ImGui::EndChild();

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
		bool should_open_node_popup = false;

		void display_scripts(Scene* scene)
		{
			for (const auto& [script_name, script] : scene->scripts) {
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
				}

				if (ImGui::BeginPopupContextItem(script_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
					bool should_break = false;
					ImGui::Text("Edit Script");

					char name_input[32] = {};
					memcpy(name_input, script_name.c_str(), std::min(script_name.size(), std::size(name_input) - 1));

					if (ImGui::InputText("##RenameScriptInput", name_input, std::size(name_input) - 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
						const std::string new_name = name_input;
						if (!scene->scripts.contains(new_name)) {
							if (this->selected_script == script_name) {
								this->selected_script = new_name;
							}
							scene->scripts[new_name] = script;
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
		{}

		void edit_interpreted_script(InterpretedScript* script)
		{

		}

		void edit_node_script(NodeScript* script)
		{
			ImNodes::BeginNodeEditor();

			// Add nodes
			if (ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				should_open_node_popup = true;
			}
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				should_open_node_popup = false;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 5.0f, 5.0f });

			if (should_open_node_popup) {
				ImGui::OpenPopup("add node");
			}
			if (ImGui::BeginPopup("add node")) {
				const ImVec2 mouse_position = ImGui::GetMousePosOnOpeningCurrentPopup();

				std::string search_buffer = {};
				search_buffer.resize(50);
				ImGui::InputText("Search", search_buffer.data(), search_buffer.size());

				const size_t search_buffer_size = search_buffer.find_first_of('\0');
				const std::string searched_info = search_buffer.substr(0, search_buffer_size);

				for (const auto& [node_name, node_function] : NODE_TYPES) {
					if (searched_info.size() > 0 && !node_name.contains(searched_info)) {
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
				ImGui::TextColored(ImColor(30, 30, 30, 255), node->title.c_str());
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
