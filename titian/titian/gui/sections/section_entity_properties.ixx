export module section_entity_properties;

export import gui_section;
export import editor_layer;
export import gui_layer;

export import camera;
export import ambient_light;
export import point_light;
export import directional_light;

export namespace titian {
    class GUISectionEntityProperties : public GUISection
    {
    public:
        EditorLayer* editor_layer = nullptr;
        GUILayer* gui_layer = nullptr;

        GUISectionEntityProperties(EditorLayer* editor_layer, GUILayer* gui_layer)
        {
            this->editor_layer = editor_layer;
            this->gui_layer = gui_layer;
        }

        ~GUISectionEntityProperties() override
        {}

        void render_gui() override
        {
            Scene* scene = &editor_layer->game_layer->scene;
            kl::Object<Entity> entity = scene->get_entity(editor_layer->selected_entity);

            if (ImGui::Begin("Entity properties") && entity) {
                display_entity_info(scene, &entity);
                edit_entity_transform(scene, &entity);
                edit_entity_mesh(scene, &entity);
                edit_entity_material(scene, &entity);
                edit_entity_physics(scene, entity);
                edit_entity_collider(scene, &entity);
            }
            ImGui::End();
        }

    private:
        void display_entity_info(Scene* scene, Entity* entity)
        {
            ImGui::Text("Entity Info");

            ImGui::Text("Name: ");
            ImGui::SameLine();
            gui_colored_text(editor_layer->selected_entity, gui_layer->special_color);

            if (Camera* camera = dynamic_cast<Camera*>(entity)) {
                ImGui::Separator();
                display_camera_special_info(scene, camera);
            }
            else if (AmbientLight* light = dynamic_cast<AmbientLight*>(entity)) {
                ImGui::Separator();
                display_ambient_light_special_info(scene, light);
            }
            else if (PointLight* light = dynamic_cast<PointLight*>(entity)) {
                ImGui::Separator();
                display_point_light_special_info(scene, light);
            }
            else if (DirectionalLight* light = dynamic_cast<DirectionalLight*>(entity)) {
                ImGui::Separator();
                display_directional_light_special_info(scene, light);
            }
        }

        void display_camera_special_info(Scene* scene, Camera* camera)
        {
            ImGui::Text("Camera Special Info");

            ImGui::DragFloat("Aspect Ratio", &camera->aspect_ratio);
            ImGui::DragFloat("FOV", &camera->field_of_view);

            ImGui::DragFloat("Near Plane", &camera->near_plane);
            ImGui::DragFloat("Far Plane", &camera->far_plane);

            ImGui::DragFloat("Sensitivity", &camera->sensitivity);
            ImGui::DragFloat("Speed", &camera->speed);

            kl::Float3 camera_forward = camera->forward();
            ImGui::DragFloat3("Forward", camera_forward);
            camera->set_forward(camera_forward);

            kl::Float3 camera_up = camera->up();
            ImGui::DragFloat3("Up", camera_up);
            camera->set_up(camera_up);

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

        void display_ambient_light_special_info(Scene* scene, AmbientLight* light)
        {
            ImGui::Text("Ambient Light Special Info");

            ImGui::ColorEdit3("Color", light->color);
            ImGui::DragFloat("Intensity", &light->intensity);
        }

        void display_point_light_special_info(Scene* scene, PointLight* light)
        {
            ImGui::Text("Point Light Special Info");

            ImGui::ColorEdit3("Color", light->color);
        }

        void display_directional_light_special_info(Scene* scene, DirectionalLight* light)
        {
            ImGui::Text("Directional Light Special Info");

            ImGui::DragFloat("Point Size", &light->point_size);
            ImGui::ColorEdit3("Color", light->color);

            kl::Float3 direction = light->direction();
            if (ImGui::DragFloat3("Direction", direction)) {
                light->set_direction(direction);
            }
        }

        void edit_entity_transform(Scene* scene, Entity* entity)
        {
            ImGui::Separator();
            ImGui::Text("Transform");

            ImGui::DragFloat3("Scale", entity->scale);

            kl::Float3 rotation = entity->rotation();
            if (ImGui::DragFloat3("Rotation", rotation)) {
                entity->set_rotation(rotation);
            }

            kl::Float3 position = entity->position();
            if (ImGui::DragFloat3("Position", position)) {
                entity->set_position(position);
            }
        }

        void edit_entity_mesh(Scene* scene, Entity* entity)
        {
            ImGui::Separator();
            ImGui::Text("Mesh");

            // Selector
            std::string& bound_mesh = entity->mesh_name;
            if (ImGui::BeginCombo("Bound Mesh", bound_mesh.c_str())) {
                const std::string filter = gui_input_continuous("Search###EntityPropsMesh");
                if (ImGui::Selectable("/", bound_mesh == "/")) {
                    bound_mesh = "/";

                    if (Collider* collider = &entity->collider()) {
                        const physx::PxGeometryType::Enum type = collider->type();
                        if (type == physx::PxGeometryType::Enum::eTRIANGLEMESH) {
                            entity->set_collider(nullptr);
                        }
                    }
                }
                for (const auto& [mesh_name, mesh] : scene->meshes) {
                    if (!filter.empty() && !mesh_name.contains(filter)) {
                        continue;
                    }
                    if (ImGui::Selectable(mesh_name.c_str(), mesh_name == bound_mesh)) {
                        bound_mesh = mesh_name;

                        if (Collider* collider = &entity->collider()) {
                            const physx::PxGeometryType::Enum type = collider->type();
                            if (type == physx::PxGeometryType::Enum::eTRIANGLEMESH) {
                                physx::PxTriangleMesh* possible_collider_mesh = mesh->physics_buffer;
                                if (possible_collider_mesh) {
                                    physx::PxTriangleMeshGeometry triangle_geometry(possible_collider_mesh, physx::PxVec3(1.0f));
                                    kl::Object collider = new Collider(scene->physics(), triangle_geometry);
                                    entity->set_collider(collider);
                                }
                                else {
                                    entity->set_collider(nullptr);
                                }
                            }
                        }
                    }
                }
                ImGui::EndCombo();
            }
        }

        void edit_entity_material(Scene* scene, Entity* entity)
        {
            ImGui::Separator();
            ImGui::Text("Material");

            // Selector
            std::string& bound_material = entity->material_name;
            if (ImGui::BeginCombo("Bound Material", bound_material.c_str())) {
                const std::string filter = gui_input_continuous("Search###EntityPropsMaterial");
                if (ImGui::Selectable("/", bound_material == "/")) {
                    bound_material = "/";
                }
                for (auto& [material, _] : scene->materials) {
                    if (!filter.empty() && !material.contains(filter)) {
                        continue;
                    }
                    if (ImGui::Selectable(material.c_str(), material == bound_material)) {
                        bound_material = material;
                    }
                }
                ImGui::EndCombo();
            }
        }

        void edit_entity_physics(Scene* scene, kl::Object<Entity>& entity)
        {
            ImGui::Separator();
            ImGui::Text("Physics");

            bool dynamic = entity->is_dynamic();
            if (ImGui::Checkbox("Dynamic", &dynamic)) {
                const std::string& name = editor_layer->selected_entity;
                scene->remove_entity(name);
                entity->set_dynamic(dynamic);
                scene->add_entity(name, entity);
            }

            if (dynamic) {
                bool gravity = entity->has_gravity();
                if (ImGui::Checkbox("Gravity", &gravity)) {
                    entity->set_gravity(gravity);
                }

                float mass = entity->mass();
                if (ImGui::DragFloat("Mass", &mass, 1.0f, 0.0f, 1e9f)) {
                    entity->set_mass(mass);
                }

                kl::Float3 velocity = entity->velocity();
                if (ImGui::DragFloat3("Velocity", velocity)) {
                    entity->set_velocity(velocity);
                }

                kl::Float3 angular = entity->angular() * kl::TO_DEGREES;
                if (ImGui::DragFloat3("Angular", angular)) {
                    entity->set_angular(angular * kl::TO_RADIANS);
                }
            }
        }

        void edit_entity_collider(Scene* scene, Entity* entity)
        {
            static const std::unordered_map<physx::PxGeometryType::Enum, std::string> possible_colliders = {
                {     physx::PxGeometryType::Enum::eINVALID, "/" },

                {     physx::PxGeometryType::Enum::eBOX,     "box" },
                {  physx::PxGeometryType::Enum::eSPHERE,  "sphere" },
                { physx::PxGeometryType::Enum::eCAPSULE, "capsule" },

                { physx::PxGeometryType::Enum::eTRIANGLEMESH,  "mesh" },
            };

            ImGui::Separator();
            ImGui::Text("Collider");

            kl::Object collider = entity->collider();
            physx::PxGeometryType::Enum collider_type = collider ? collider->type() : physx::PxGeometryType::Enum::eINVALID;
            std::string collider_name = possible_colliders.at(collider_type);

            // Choose type
            if (ImGui::BeginCombo("Bound Collider", collider_name.c_str())) {
                for (auto& [type, name] : possible_colliders) {
                    if (ImGui::Selectable(name.c_str(), type == collider_type)) {
                        Mesh* mesh = &scene->get_mesh(entity->mesh_name);
                        collider = scene->new_default_collider(type, mesh);
                        collider_type = (collider ? collider->type() : physx::PxGeometryType::Enum::eINVALID);
                        collider_name = possible_colliders.at(collider_type);
                        entity->set_collider(collider);
                    }
                }

                ImGui::EndCombo();
            }
            if (!collider) {
                return;
            }

            // General info
            float restitution = collider->restitution();
            if (ImGui::DragFloat("Restitution", &restitution, 0.1f, 0.0f, 1e9f)) {
                collider->set_restitution(restitution);
            }

            float static_friction = collider->static_friction();
            if (ImGui::DragFloat("Static Friction", &static_friction, 0.1f, 0.0f, 1e9f)) {
                collider->set_static_friction(static_friction);
            }

            float dynamic_friction = collider->dynamic_friction();
            if (ImGui::DragFloat("Dynamic Friction", &dynamic_friction, 0.1f, 0.0f, 1e9f)) {
                collider->set_dynamic_friction(dynamic_friction);
            }

            // Specific info
            int geometry_type = 0;
            physx::PxBoxGeometry box_geometry = {};
            physx::PxTriangleMeshGeometry mesh_geometry = {};
            physx::PxShape* collider_shape = collider->shape();
            if (collider_type == physx::PxGeometryType::Enum::eBOX) {
                collider_shape->getBoxGeometry(box_geometry);

                if (ImGui::DragFloat3("Size", reinterpret_cast<float*>(&box_geometry.halfExtents), 0.5f, 0.0f, 1e9f)) {
                    collider_shape->setGeometry(box_geometry);
                }
                geometry_type = 1;
            }
            else if (collider_type == physx::PxGeometryType::Enum::eSPHERE) {
                physx::PxSphereGeometry geometry = {};
                collider_shape->getSphereGeometry(geometry);

                if (ImGui::DragFloat("Radius", &geometry.radius, 0.5f, 0.0f, 1e9f)) {
                    collider_shape->setGeometry(geometry);
                }
            }
            else if (collider_type == physx::PxGeometryType::Enum::eCAPSULE) {
                physx::PxCapsuleGeometry geometry = {};
                collider_shape->getCapsuleGeometry(geometry);

                if (ImGui::DragFloat("Radius", &geometry.radius, 0.5f, 0.0f, 1e9f)) {
                    collider_shape->setGeometry(geometry);
                }
                if (ImGui::DragFloat("Height", &geometry.halfHeight, 0.5f, 0.0f, 1e9f)) {
                    collider_shape->setGeometry(geometry);
                }
            }
            else if (collider_type == physx::PxGeometryType::Enum::eTRIANGLEMESH) {
                collider_shape->getTriangleMeshGeometry(mesh_geometry);

                if (ImGui::DragFloat3("Mesh Scale", reinterpret_cast<float*>(&mesh_geometry.scale), 0.5f, 0.0f, 1e9f)) {
                    collider_shape->setGeometry(mesh_geometry);
                }
                geometry_type = 2;
            }

            kl::Float3 rotation = collider->rotation();
            if (ImGui::DragFloat3("Offset Rotation", rotation)) {
                collider->set_rotation(rotation);
            }

            kl::Float3 offset = collider->offset();
            if (ImGui::DragFloat3("Offset Position", offset)) {
                collider->set_offset(offset);
            }

            // Loading buttons
            if (geometry_type != 0 && ImGui::Button("Load size from scale")) {
                if (geometry_type == 1) {
                    box_geometry.halfExtents = reinterpret_cast<physx::PxVec3&>(entity->scale);
                    collider_shape->setGeometry(box_geometry);
                }
                else {
                    mesh_geometry.scale = reinterpret_cast<physx::PxVec3&>(entity->scale);
                    collider_shape->setGeometry(mesh_geometry);
                }
            }
        }
    };
}
