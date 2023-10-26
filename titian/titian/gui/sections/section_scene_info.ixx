export module section_scene_info;

export import gui_section;
export import editor_layer;

export namespace titian {
	class GUISectionSceneInfo : public GUISection
	{
	public:
        kl::Object<EditorLayer> editor_layer = nullptr;

		GUISectionSceneInfo(kl::Object<EditorLayer>& editor_layer)
		{
            this->editor_layer = editor_layer;
        }

		~GUISectionSceneInfo() override
		{}

		void render_gui() override
		{
            if (ImGui::Begin("Scene Info")) {
                Scene* scene = &editor_layer->game_layer->scene;

                int entity_count = (int) scene->entity_count();
                ImGui::DragInt("Entity count", &entity_count);

                kl::Float3 gravity = scene->gravity();
                if (ImGui::DragFloat3("Gravity", gravity)) {
                    scene->set_gravity(gravity);
                }

                Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
                if (camera) {
                    kl::Float3 camera_position = camera->position();
                    ImGui::DragFloat3("Camera position", camera_position);
                    camera->set_position(camera_position);

                    kl::Float3 camera_direction = camera->forward();
                    ImGui::DragFloat3("Camera direction", camera_direction);
                    camera->set_forward(camera_direction);

                    // Skybox
                    if (ImGui::BeginCombo("Bound Skybox", camera->skybox_name.c_str())) {
                        if (ImGui::Selectable("/", camera->skybox_name == "/")) {
                            camera->skybox_name = "/";
                        }
                        for (auto& [texture_name, _] : scene->textures) {
                            if (ImGui::Selectable(texture_name.c_str(), texture_name == camera->skybox_name)) {
                                camera->skybox_name = texture_name;
                            }
                        }
                        ImGui::EndCombo();
                    }

                    // Background
                    if (!scene->textures.contains(camera->skybox_name)) {
                        kl::Float4 background = camera->background;
                        if (ImGui::ColorEdit4("Background", background)) {
                            camera->background = background;
                        }
                    }
                }
            }
            ImGui::End();
		}
	};
}
