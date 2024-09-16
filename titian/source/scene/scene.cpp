#include "titian.h"


px::PxDefaultAllocator     titian::Scene::m_allocator = {};
px::PxDefaultErrorCallback titian::Scene::m_error_callback = {};
px::PxFoundation*          titian::Scene::m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);

static px::PxFilterFlags filter_shader(
    px::PxFilterObjectAttributes attributes0,
    px::PxFilterData filterData0,
    px::PxFilterObjectAttributes attributes1,
    px::PxFilterData filterData1,
    px::PxPairFlags& pairFlags,
    const void* constantBlock,
    px::PxU32 constantBlockSize)
{
    pairFlags = px::PxPairFlag::eCONTACT_DEFAULT;
    pairFlags |= px::PxPairFlag::eNOTIFY_TOUCH_FOUND;
    return px::PxFilterFlag::eDEFAULT;
}

titian::Scene::Scene(kl::GPU* gpu)
    : m_gpu(gpu)
{
    m_dispatcher = px::PxDefaultCpuDispatcherCreate(2);
    kl::assert(m_dispatcher, "Failed to create physics dispatcher");

    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, px::PxTolerancesScale{});
    kl::assert(m_physics, "Failed to create physics");

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, px::PxCookingParams{ m_physics->getTolerancesScale() });
    kl::assert(m_cooking, "Failed to create physics cooking");

    px::PxSceneDesc scene_descriptor = { m_physics->getTolerancesScale() };
    scene_descriptor.gravity.y = -9.81f;
    scene_descriptor.cpuDispatcher = m_dispatcher;
    scene_descriptor.filterShader = filter_shader;
    scene_descriptor.simulationEventCallback = this;

    m_scene = m_physics->createScene(scene_descriptor);
    kl::assert(m_scene, "Failed to create physics scene");

    default_meshes = new DefaultMeshes(this);
    kl::assert(default_meshes, "Failed to init default meshes");

    default_animations = new DefaultAnimations(m_gpu, this);
    kl::assert(default_animations, "Failed to init default animations");

    default_materials = new DefaultMaterials(gpu);
    kl::assert(default_materials, "Failed to init default materials");
}

titian::Scene::~Scene()
{
    default_meshes.free();
    default_materials.free();

    meshes.clear();
    textures.clear();
    materials.clear();
    scripts.clear();

    while (!m_entities.empty()) {
        remove_entity(m_entities.begin()->first);
    }

    m_scene->release();
    m_dispatcher->release();
    m_cooking->release();
    m_physics->release();
}

void titian::Scene::serialize(Serializer* serializer, const void* helper_data) const
{
    serializer->write_string("main_camera_name", main_camera_name);
    serializer->write_string("main_ambient_light_name", main_ambient_light_name);
    serializer->write_string("main_directional_light_name", main_directional_light_name);

    const auto write_map = [&]<typename T>(const StringView& map_name, const StringMap<Ref<T>>& data, const void* helper_data)
    {
        serializer->push_object(map_name);
        serializer->write_int("data_size", (int32_t) data.size());
        int counter = 0;
        for (auto& [name, object] : data) {
            serializer->write_string(kl::format("__name_", counter), name);
            serializer->push_object(name);
            object->serialize(serializer, helper_data);
            serializer->pop_object();
            counter += 1;
        }
        serializer->pop_object();
    };

    write_map("meshes", meshes, nullptr);
    write_map("animations", animations, nullptr);
    write_map("textures", textures, nullptr);
    write_map("materials", materials, nullptr);
    write_map("shaders", shaders, nullptr);
    write_map("scripts", scripts, nullptr);
    write_map("entities", m_entities, &meshes);
}

void titian::Scene::deserialize(const Serializer* serializer, const void* helper_data)
{
    serializer->read_string("main_camera_name", main_camera_name);
    serializer->read_string("main_ambient_light_name", main_ambient_light_name);
    serializer->read_string("main_directional_light_name", main_directional_light_name);

    const auto read_map = [&]<typename T>(const StringView& map_name, StringMap<Ref<T>>& data, const Function<T*()>& provider, const void* helper_data)
    {
        serializer->load_object(map_name);
        int32_t data_size = 0;
        serializer->read_int("data_size", data_size);
        for (int32_t i = 0; i < data_size; i++) {
            String name;
            serializer->read_string(kl::format("__name_", i), name);
            Ref<T> object = provider();
            serializer->load_object(name);
            object->deserialize(serializer, helper_data);
            serializer->unload_object();
            data[name] = object;
        }
        serializer->unload_object();
    };

    Function mesh_provider = [&] { return new Mesh(this, m_gpu); };
    read_map("meshes", meshes, mesh_provider, nullptr);

    Function animation_provider = [&] { return new Animation(this, m_gpu); };
    read_map("animations", animations, animation_provider, nullptr);

    Function texture_provider = [&] { return new Texture(m_gpu); };
    read_map("textures", textures, texture_provider, nullptr);

    Function material_provider = [&] { return new Material(); };
    read_map("materials", materials, material_provider, nullptr);

    Function shader_provider = [&] { return new Shader(m_gpu, ShaderType::MATERIAL); };
    read_map("shaders", shaders, shader_provider, nullptr);

    {
        serializer->load_object("scripts");
        int32_t data_size = 0;
        serializer->read_int("data_size", data_size);
        for (int32_t i = 0; i < data_size; i++) {
            String name;
            serializer->read_string(kl::format("__name_", i), name);
            serializer->load_object(name);
            String script_type;
            serializer->read_string("script_type", script_type);
            Ref<Script> script;
            if (typeid(InterpScript).name() == script_type) {
                script = new InterpScript();
            }
            else if (typeid(NodeScript).name() == script_type) {
                script = new NodeScript();
            }
            else if (typeid(NativeScript).name() == script_type) {
                script = new NativeScript();
            }
            else {
                kl::assert(false, "Unknown script type: ", script_type);
            }
            script->deserialize(serializer, nullptr);
            serializer->unload_object();
            scripts[name] = script;
        }
        serializer->unload_object();
    }

    {
        serializer->load_object("entities");
        int32_t data_size = 0;
        serializer->read_int("data_size", data_size);
        for (int32_t i = 0; i < data_size; i++) {
            String name;
            serializer->read_string(kl::format("__name_", i), name);
            serializer->load_object(name);
            String entity_type;
            serializer->read_string("entity_type", entity_type);
            Ref<Entity> entity;
            if (typeid(Entity).name() == entity_type) {
                entity = new Entity(m_physics);
            }
            else if (typeid(Camera).name() == entity_type) {
                entity = new Camera(m_physics, m_gpu);
            }
            else if (typeid(AmbientLight).name() == entity_type) {
                entity = new AmbientLight(m_physics);
            }
            else if (typeid(PointLight).name() == entity_type) {
                entity = new PointLight(m_physics);
            }
            else if (typeid(DirectionalLight).name() == entity_type) {
                entity = new DirectionalLight(m_physics, m_gpu);
            }
            else {
                kl::assert(false, "Unknown entity type: ", entity_type);
            }
            entity->deserialize(serializer, &meshes);
            serializer->unload_object();
            add_entity(name, entity);
        }
        serializer->unload_object();
    }
}

void titian::Scene::onConstraintBreak(px::PxConstraintInfo* constraints, px::PxU32 count)
{}

void titian::Scene::onWake(px::PxActor** actors, px::PxU32 count)
{}

void titian::Scene::onSleep(px::PxActor** actors, px::PxU32 count)
{}

void titian::Scene::onContact(const px::PxContactPairHeader& pairHeader, const px::PxContactPair* pairs, px::PxU32 nbPairs)
{
    Entity* entity_0 = (Entity*) pairHeader.actors[0]->userData;
    Entity* entity_1 = (Entity*) pairHeader.actors[1]->userData;
    if (entity_0 && entity_1) {
        for (auto& [_, script] : scripts) {
            script->call_collision(this, entity_0, entity_1);
        }
    }
}

void titian::Scene::onTrigger(px::PxTriggerPair* pairs, px::PxU32 count)
{}

void titian::Scene::onAdvance(const px::PxRigidBody* const* bodyBuffer, const px::PxTransform* poseBuffer, const px::PxU32 count)
{}

px::PxPhysics* titian::Scene::physics() const
{
    return m_physics;
}

px::PxCooking* titian::Scene::cooking() const
{
    return m_cooking;
}

void titian::Scene::set_gravity(const Float3& gravity)
{
    m_scene->setGravity(reinterpret_cast<const px::PxVec3&>(gravity));
}

titian::Float3 titian::Scene::gravity() const
{
    const px::PxVec3 gravity = m_scene->getGravity();
    return reinterpret_cast<const Float3&>(gravity);
}

void titian::Scene::update_physics(const float delta_t)
{
    m_scene->simulate(delta_t);
    m_scene->fetchResults(true);
}

void titian::Scene::update_scripts()
{
    for (auto& [_, script] : scripts) {
        script->call_update(this);
    }
}

void titian::Scene::update_ui()
{
    for (auto& [_, script] : scripts) {
        script->call_ui(this);
    }
}

titian::Ref<titian::Entity> titian::Scene::new_entity() const
{
    return new Entity(m_physics);
}

void titian::Scene::add_entity(const String& id, const Ref<Entity>& entity)
{
    m_entities[id] = entity;
    m_scene->addActor(*entity->actor());
}

void titian::Scene::remove_entity(const StringView& id)
{
    const auto it = m_entities.find(id);
    if (it == m_entities.end())
        return;

    m_scene->removeActor(*it->second->actor());
    m_entities.erase(it);
}

titian::Ref<titian::Collider> titian::Scene::new_box_collider(const Float3& scale) const
{
    Ref result = new Collider(m_physics);
    result->set_geometry(px::PxBoxGeometry{ reinterpret_cast<const px::PxVec3&>(scale) * 0.5f });
    return result;
}

titian::Ref<titian::Collider> titian::Scene::new_sphere_collider(const float radius) const
{
    Ref result = new Collider(m_physics);
    result->set_geometry(px::PxSphereGeometry{ radius });
    return result;
}

titian::Ref<titian::Collider> titian::Scene::new_capsule_collider(const float radius, const float height) const
{
    Ref result = new Collider(m_physics);
    result->set_geometry(px::PxCapsuleGeometry{ radius, height });
    return result;
}

titian::Ref<titian::Collider> titian::Scene::new_mesh_collider(const Mesh& mesh, const Float3& scale) const
{
    Ref result = new Collider(m_physics);
    if (mesh.physics_buffer) {
        result->set_geometry(px::PxTriangleMeshGeometry{ mesh.physics_buffer, reinterpret_cast<const px::PxVec3&>(scale) });
    }
    return result;
}

titian::Ref<titian::Collider> titian::Scene::new_default_collider(const px::PxGeometryType::Enum type, const Mesh* optional_mesh) const
{
    switch (type)
    {
    case px::PxGeometryType::Enum::eBOX: return new_box_collider(Float3{ 1.0f });
    case px::PxGeometryType::Enum::eSPHERE: return new_sphere_collider(1.0f);
    case px::PxGeometryType::Enum::eCAPSULE: return new_capsule_collider(1.0f, 2.0f);
    case px::PxGeometryType::Enum::eTRIANGLEMESH: if (optional_mesh) return new_mesh_collider(*optional_mesh, Float3{ 1.0f });
    }
    return nullptr;
}

titian::Mesh* titian::Scene::helper_new_mesh(const String& id)
{
    Mesh* mesh = new Mesh(this, m_gpu);
    meshes[id] = mesh;
    return mesh;
}

titian::Animation* titian::Scene::helper_new_animation(const String& id)
{
    Animation* animation = new Animation(this, m_gpu);
    animations[id] = animation;
    return animation;
}

titian::Texture* titian::Scene::helper_new_texture(const String& id)
{
    Texture* texture = new Texture(m_gpu);
    textures[id] = texture;
    return texture;
}

titian::Material* titian::Scene::helper_new_material(const String& id)
{
    Material* material = new Material();
    materials[id] = material;
    return material;
}

titian::Shader* titian::Scene::helper_new_shader(const String& id)
{
    Shader* shader = new Shader(m_gpu, ShaderType::MATERIAL);
    shaders[id] = shader;
    return shader;
}

titian::Entity* titian::Scene::helper_new_entity(const String& id)
{
    Ref entity = new_entity();
    add_entity(id, entity);
    return &entity;
}

titian::Mesh* titian::Scene::helper_get_mesh(const StringView& id)
{
    const auto it = meshes.find(id);
    if (it != meshes.end()) {
        return &it->second;
    }
    return nullptr;
}

titian::Animation* titian::Scene::helper_get_animation(const StringView& id)
{
    const auto it = animations.find(id);
    if (it != animations.end()) {
        return &it->second;
    }
    return nullptr;
}

titian::Texture* titian::Scene::helper_get_texture(const StringView& id)
{
    const auto it = textures.find(id);
    if (it != textures.end()) {
        return &it->second;
    }
    return nullptr;
}

titian::Material* titian::Scene::helper_get_material(const StringView& id)
{
    const auto it = materials.find(id);
    if (it != materials.end()) {
        return &it->second;
    }
    return nullptr;
}

titian::Shader* titian::Scene::helper_get_shader(const StringView& id)
{
    const auto it = shaders.find(id);
    if (it != shaders.end()) {
        return &it->second;
    }
    return nullptr;
}

titian::Entity* titian::Scene::helper_get_entity(const StringView& id)
{
    const auto it = m_entities.find(id);
    if (it != m_entities.end()) {
        return &it->second;
    }
    return nullptr;
}

void titian::Scene::helper_remove_mesh(const StringView& id)
{
    const auto it = meshes.find(id);
    if (it != meshes.end()) {
        meshes.erase(it);
    }
}

void titian::Scene::helper_remove_animation(const StringView& id)
{
    const auto it = animations.find(id);
    if (it != animations.end()) {
        animations.erase(it);
    }
}

void titian::Scene::helper_remove_texture(const StringView& id)
{
    const auto it = textures.find(id);
    if (it != textures.end()) {
        textures.erase(it);
    }
}

void titian::Scene::helper_remove_material(const StringView& id)
{
    const auto it = materials.find(id);
    if (it != materials.end()) {
        materials.erase(it);
    }
}

void titian::Scene::helper_remove_shader(const StringView& id)
{
    const auto it = shaders.find(id);
    if (it != shaders.end()) {
        shaders.erase(it);
    }
}

void titian::Scene::helper_remove_entity(const StringView& id)
{
    remove_entity(id);
}

bool titian::Scene::helper_contains_mesh(const StringView& id) const
{
    return meshes.contains(id);
}

bool titian::Scene::helper_contains_animation(const StringView& id) const
{
    return animations.contains(id);
}

bool titian::Scene::helper_contains_texture(const StringView& id) const
{
    return textures.contains(id);
}

bool titian::Scene::helper_contains_material(const StringView& id) const
{
    return materials.contains(id);
}

bool titian::Scene::helper_contains_shader(const StringView& id) const
{
    return shaders.contains(id);
}

bool titian::Scene::helper_contains_entity(const StringView& id) const
{
    return m_entities.contains(id);
}

void titian::Scene::helper_iterate_meshes(const Function<void(const String&, Mesh*)>& func)
{
    std::for_each(meshes.begin(), meshes.end(), [&](auto& entry) { func(entry.first, &entry.second); });
}

void titian::Scene::helper_iterate_animations(const Function<void(const String&, Animation*)>& func)
{
    std::for_each(animations.begin(), animations.end(), [&](auto& entry) { func(entry.first, &entry.second); });
}

void titian::Scene::helper_iterate_textures(const Function<void(const String&, Texture*)>& func)
{
    std::for_each(textures.begin(), textures.end(), [&](auto& entry) { func(entry.first, &entry.second); });
}

void titian::Scene::helper_iterate_materials(const Function<void(const String&, Material*)>& func)
{
    std::for_each(materials.begin(), materials.end(), [&](auto& entry) { func(entry.first, &entry.second); });
}

void titian::Scene::helper_iterate_shaders(const Function<void(const String&, Shader*)>& func)
{
    std::for_each(shaders.begin(), shaders.end(), [&](auto& entry) { func(entry.first, &entry.second); });
}

void titian::Scene::helper_iterate_entities(const Function<void(const String&, Entity*)>& func)
{
    std::for_each(m_entities.begin(), m_entities.end(), [&](auto& entry) { func(entry.first, &entry.second); });
}

titian::Optional<titian::AssimpData> titian::Scene::get_assimp_data(const StringView& path) const
{
    Ref importer = new as::Importer();
    const aiScene* scene = importer->ReadFile(path,
        aiProcess_ConvertToLeftHanded |
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_GenNormals);
    if (!scene)
        return std::nullopt;

    AssimpData data{};
    data.importer = importer;
    for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        String name = mesh->mName.C_Str();
        if (name.empty()) {
            name = "unknown_mesh";
        }
        data.meshes.push_back(generate_unique_name(name, meshes));
    }
	for (uint32_t i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation* animation = scene->mAnimations[i];
        String name = animation->mName.C_Str();
        if (name.empty()) {
            name = "unknown_animation";
        }
        data.animations.push_back(generate_unique_name(name, animations));
	}
	for (uint32_t i = 0; i < scene->mNumTextures; i++) {
		aiTexture* texture = scene->mTextures[i];
        String name = texture->mFilename.C_Str();
		if (name.empty()) {
			name = "unknown_texture";
		}
        data.textures.push_back(generate_unique_name(name, textures));
	}
    for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];
		String name = material->GetName().C_Str();
		if (name.empty()) {
			name = "unknown_material";
		}
        data.materials.push_back(generate_unique_name(name, materials));
    }
    return data;
}

void titian::Scene::load_assimp_data(const AssimpData& data)
{
    const aiScene* scene = data.importer->GetScene();
    for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
        meshes[data.meshes[i]] = load_assimp_mesh(scene, scene->mMeshes[i]);
    }
    for (uint32_t i = 0; i < scene->mNumAnimations; i++) {
        animations[data.animations[i]] = load_assimp_animation(scene, scene->mAnimations[i]);
    }
    for (uint32_t i = 0; i < scene->mNumTextures; i++) {
        textures[data.textures[i]] = load_assimp_texture(scene, scene->mTextures[i]);
    }
    for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
		materials[data.materials[i]] = load_assimp_material(scene, scene->mMaterials[i]);
    }
}

titian::Ref<titian::Mesh> titian::Scene::load_assimp_mesh(const aiScene* scene, const aiMesh* mesh)
{
    Ref mesh_object = new Mesh(this, m_gpu);
    
    Vector<Vertex> vertices(mesh->mNumVertices);
    if (mesh->HasPositions()) {
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            vertices[i].world = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        }
    }
    if (mesh->HasTextureCoords(0)) {
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            vertices[i].texture = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }
    }
    if (mesh->HasNormals()) {
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            vertices[i].normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        }
    }
    if (mesh->HasBones()) {
        if (mesh->mNumBones > MAX_BONE_COUNT) {
            Logger::log("Mesh has too many bones: ", mesh->mNumBones, " > ", MAX_BONE_COUNT);
        }

        for (uint32_t i = 0; i < mesh->mNumBones; i++) {
            auto& bone = mesh->mBones[i];
            for (uint32_t j = 0; j < bone->mNumWeights; j++) {
                auto& weight = bone->mWeights[j];
                auto& vertex = vertices[weight.mVertexId];
                for (int k = 0; k < MAX_BONE_REFS; k++) {
                    if (vertex.bone_weights[k] == 0.0f) {
                        vertex.bone_weights[k] = (float) weight.mWeight;
                        vertex.bone_indices[k] = (uint8_t) i;
                        break;
                    }
                }
            }
        }

        mesh_object->bone_matrices.resize(mesh->mNumBones);
        for (uint32_t i = 0; i < mesh->mNumBones; i++) {
            auto& bone = mesh->mBones[i];
            auto& offset_matrix = mesh_object->bone_matrices[i];
            for (int j = 0; j < 4; j++) {
                memcpy(&offset_matrix(0, j), bone->mOffsetMatrix[j], 4 * sizeof(float));
            }
        }

        Function<Ref<SkeletonNode>(const aiNode*)> recur_helper;
        recur_helper = [&](const aiNode* node)
        {
            Ref<SkeletonNode> skeleton_node = new SkeletonNode();
            skeleton_node->bone_index = -1;
            for (uint32_t i = 0; i < mesh->mNumBones; i++) {
				if (mesh->mBones[i]->mName == node->mName) {
                    skeleton_node->bone_index = i;
					break;
				}
            }
            for (int i = 0; i < 4; i++) {
                memcpy(&skeleton_node->transformation(0, i), node->mTransformation[i], 4 * sizeof(float));
            }
            skeleton_node->children.resize(node->mNumChildren);
			for (uint32_t i = 0; i < node->mNumChildren; i++) {
                skeleton_node->children[i] = recur_helper(node->mChildren[i]);
			}
            return skeleton_node;
        };
        mesh_object->skeleton_root = recur_helper(scene->mRootNode);
    }

    mesh_object->data_buffer.reserve((size_t) mesh->mNumFaces * 3);
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        const auto& face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            const uint32_t index = face.mIndices[j];
            mesh_object->data_buffer.push_back(vertices[index]);
        }
    }

    mesh_object->reload();
    return mesh_object;
}

titian::Ref<titian::Animation> titian::Scene::load_assimp_animation(const aiScene* scene, const aiAnimation* animation)
{
    Ref animation_object = new Animation(this, m_gpu);

    animation_object->ticks_per_second = (float) animation->mTicksPerSecond;
    animation_object->duration_in_ticks = (float) animation->mDuration;

    animation_object->channels.resize(animation->mNumChannels);
    for (uint32_t i = 0; i < animation->mNumChannels; i++) {
        auto& channel = animation->mChannels[i];
        auto& anim_channel = animation_object->channels[i];
        
        anim_channel.scalings.resize(channel->mNumScalingKeys);
        for (uint32_t j = 0; j < channel->mNumScalingKeys; j++) {
            auto& key = channel->mScalingKeys[j];
            anim_channel.scalings[j].first = (float) key.mTime;
            anim_channel.scalings[j].second = key.mValue;
        }

		anim_channel.rotations.resize(channel->mNumRotationKeys);
        for (uint32_t j = 0; j < channel->mNumRotationKeys; j++) {
            auto& key = channel->mRotationKeys[j];
            anim_channel.rotations[j].first = (float) key.mTime;
			anim_channel.rotations[j].second = key.mValue;
        }

		anim_channel.positions.resize(channel->mNumPositionKeys);
		for (uint32_t j = 0; j < channel->mNumPositionKeys; j++) {
			auto& key = channel->mPositionKeys[j];
			anim_channel.positions[j].first = (float) key.mTime;
			anim_channel.positions[j].second = key.mValue;
		}
	}

    Function<Ref<AnimationNode>(const aiNode*)> recur_helper;
    recur_helper = [&](const aiNode* node)
    {
        Ref<AnimationNode> animation_node = new AnimationNode();
        animation_node->channel_index = -1;
        for (uint32_t i = 0; i < animation->mNumChannels; i++) {
            if (animation->mChannels[i]->mNodeName == node->mName) {
                animation_node->channel_index = i;
                break;
            }
        }
        animation_node->children.resize(node->mNumChildren);
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            animation_node->children[i] = recur_helper(node->mChildren[i]);
        }
        return animation_node;
    };
    animation_object->animation_root = recur_helper(scene->mRootNode);

    return animation_object;
}

titian::Ref<titian::Texture> titian::Scene::load_assimp_texture(const aiScene* scene, const aiTexture* texture)
{
    Ref texture_object = new Texture(m_gpu);

    if (texture->mHeight == 0) {
        texture_object->data_buffer.load_from_memory((byte*) texture->pcData, texture->mWidth);
	}
    else {
        texture_object->data_buffer.resize({ (int) texture->mWidth, (int) texture->mHeight });
        memcpy(texture_object->data_buffer.ptr(), texture->pcData, (size_t) texture->mWidth * texture->mHeight * 4);
    }

    texture_object->reload_as_2D();
    texture_object->create_shader_view();

    return texture_object;
}

titian::Ref<titian::Material> titian::Scene::load_assimp_material(const aiScene* scene, const aiMaterial* material)
{
	Ref material_object = new Material();
    material->Get(AI_MATKEY_COLOR_DIFFUSE, material_object->color);
	material->Get(AI_MATKEY_COLOR_TRANSPARENT, material_object->color.w);
	material->Get(AI_MATKEY_REFLECTIVITY, material_object->reflection_factor);
	material->Get(AI_MATKEY_REFRACTI, material_object->refraction_index);
	return material_object;
}
