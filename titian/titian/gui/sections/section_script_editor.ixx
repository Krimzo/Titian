export module section_script_editor;

export import gui_section;
export import editor_layer;
export import visual_script;

export namespace titian {
	class GUISectionScriptEditor : public GUISection
	{
	public:
		kl::Object<EditorLayer> editor_layer = nullptr;

		std::string selected_script = "/";

		GUISectionScriptEditor(kl::Object<EditorLayer>& editor_layer)
		{
			this->editor_layer = editor_layer;
		}

		~GUISectionScriptEditor() override
		{}

		void render_gui() override
		{
			kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
			Scene* scene = &editor_layer->game_layer->scene;
			VisualScript* script = dynamic_cast<VisualScript*>(&scene->get_script(selected_script));

			if (ImGui::Begin("Script Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
				const float available_width = ImGui::GetContentRegionAvail().x;
				ImGui::Columns(2, "ScriptEditorColumns", false);

				ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
				if (ImGui::BeginChild("ScriptListChild")) {
					if (ImGui::BeginPopupContextWindow()) {
						if (ImGui::Button("New Visual Script")) {
							std::string possible_name = "new_script";
							for (int i = 0; scene->scripts.contains(possible_name); i++) {
								possible_name = kl::format("new_script", i);
							}
							scene->scripts[possible_name] = new VisualScript(scene);
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

				if (ImGui::BeginChild("ScriptEditorChild") && script) {
					ImNodes::BeginNodeEditor();

					render_node(script->start_node);
					render_node(script->update_node);

					for (const auto& node : script->nodes) {
						render_node(node);
					}

					ImNodes::MiniMap();
					ImNodes::EndNodeEditor();
				}
				ImGui::EndChild();

				ImGui::PopStyleVar(2);
			}
			ImGui::End();
		}

	private:
		void display_scripts(Scene* scene)
		{
			for (const auto& [script_name, script] : scene->scripts) {
				const bool is_visual_script = static_cast<bool>(dynamic_cast<const VisualScript*>(&script));

				if (is_visual_script) {
					if (ImGui::Selectable(script_name.c_str(), script_name == this->selected_script)) {
						this->selected_script = script_name;
					}
				}
				else {
					ImGui::Text(script_name.c_str());
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

		void render_node(const VisualNode& node)
		{
			ImNodes::BeginNode(node.id);

			ImNodes::BeginNodeTitleBar();
			ImGui::TextUnformatted(node.title.c_str());
			ImNodes::EndNodeTitleBar();

			for (const auto& pin : node.pins) {
				render_pin(pin);
			}

			ImNodes::EndNode();
		}

		void render_pin(const VisualPin& pin)
		{
			pin.kind == VisualPin::Kind::INPUT ? ImNodes::BeginInputAttribute(pin.id) : ImNodes::BeginOutputAttribute(pin.id);

			ImGui::Text(pin.name.c_str());

			pin.kind == VisualPin::Kind::INPUT ? ImNodes::EndInputAttribute() : ImNodes::EndOutputAttribute();
		}
	};
}
