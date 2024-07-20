#include "main.h"


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
    m_dispatcher = px::PxDefaultCpuDispatcherCreate(kl::CPU_CORE_COUNT / 4);
    kl::assert(m_dispatcher, "Failed to create physics dispatcher");

    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, px::PxTolerancesScale());
    kl::assert(m_physics, "Failed to create physics");

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, px::PxCookingParams(m_physics->getTolerancesScale()));
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
        this->remove_entity(m_entities.begin()->first);
    }

    m_scene->release();
    m_dispatcher->release();
    m_cooking->release();
    m_physics->release();
}

// Overrides
void titian::Scene::serialize(Serializer* serializer, const void* helper_data) const
{
    serializer->write_string(main_camera_name);
    serializer->write_string(main_ambient_light_name);
    serializer->write_string(main_directional_light_name);

    auto write_map = [&]<typename T>(const Map<String, Ref<T>>& data, const void* helper_data)
    {
        serializer->write_object<uint64_t>(data.size());
        for (auto& [name, object] : data) {
            serializer->write_string(name);
            object->serialize(serializer, helper_data);
        }
    };

    write_map(meshes, nullptr);
    write_map(animations, nullptr);
    write_map(textures, nullptr);
    write_map(materials, nullptr);
    write_map(shaders, nullptr);
    write_map(scripts, nullptr);
    write_map(m_entities, &meshes);
}

void titian::Scene::deserialize(const Serializer* serializer, const void* helper_data)
{
    serializer->read_string(main_camera_name);
    serializer->read_string(main_ambient_light_name);
    serializer->read_string(main_directional_light_name);

    auto read_map = [&]<typename T>(Map<String, Ref<T>>& data, const Function<T*()>& provider, const void* helper_data)
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const String name = serializer->read_string();
            Ref<T> object = provider();
            object->deserialize(serializer, helper_data);
            data[name] = object;
        }
    };

    Function mesh_provider = [&] { return new Mesh(m_gpu, m_physics, m_cooking); };
    read_map(meshes, mesh_provider, nullptr);

    Function animation_provider = [&] { return new Animation(m_gpu, this); };
    read_map(animations, animation_provider, nullptr);

    Function texture_provider = [&] { return new Texture(m_gpu); };
    read_map(textures, texture_provider, nullptr);

    Function material_provider = [&] { return new Material(); };
    read_map(materials, material_provider, nullptr);

    Function shader_provider = [&] { return new Shader(ShaderType::MATERIAL, m_gpu); };
    read_map(shaders, shader_provider, nullptr);

    /* SCRIPTS */
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const String name = serializer->read_string();
            const ScriptType type = serializer->read_object<ScriptType>();

            Ref<Script> object = nullptr;
            switch (type) {
            case ScriptType::NATIVE:
                object = new NativeScript();
                break;
            case ScriptType::INTERP:
                object = new InterpScript();
                break;
            case ScriptType::NODE:
                object = new NodeScript();
                break;
            default:
                kl::assert(false, "Unknown script type: ", (int) type);
            }
            object->deserialize(serializer, nullptr);
            scripts[name] = object;
        }
    }

    /* ENTITIES */
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const String name = serializer->read_string();
            const EntityType type = serializer->read_object<EntityType>();

            Ref<Entity> entity = nullptr;
            switch (type) {
            case EntityType::BASIC:
                entity = new Entity(EntityType::BASIC, m_physics, false);
                break;
            case EntityType::CAMERA:
                entity = new Camera(m_physics, false);
                break;
            case EntityType::AMBIENT_LIGHT:
                entity = new AmbientLight(m_physics, false);
                break;
            case EntityType::POINT_LIGHT:
                entity = new PointLight(m_physics, false);
                break;
            case EntityType::DIRECTIONAL_LIGHT:
                entity = new DirectionalLight(m_physics, false, m_gpu, 4096);
                break;
            default:
                kl::assert(false, "Unknown entity type: ", (int) type);
            }

            entity->deserialize(serializer, &meshes);
            this->add_entity(name, entity);
        }
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
    Entity* entity_0 = static_cast<Entity*>(pairHeader.actors[0]->userData);
    Entity* entity_1 = static_cast<Entity*>(pairHeader.actors[1]->userData);
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

// Get ptrs
px::PxPhysics* titian::Scene::physics() const
{
    return m_physics;
}

px::PxCooking* titian::Scene::cooking() const
{
    return m_cooking;
}

// Gravity
void titian::Scene::set_gravity(const Float3& gravity)
{
    m_scene->setGravity(reinterpret_cast<const px::PxVec3&>(gravity));
}

titian::Float3 titian::Scene::gravity() const
{
    const px::PxVec3 gravity = m_scene->getGravity();
    return reinterpret_cast<const Float3&>(gravity);
}

// Update
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

// Entity
titian::Ref<titian::Entity> titian::Scene::new_entity(const bool dynamic) const
{
    return new Entity(EntityType::BASIC, m_physics, dynamic);
}

void titian::Scene::add_entity(const String& name, const Ref<Entity>& entity)
{
    m_entities[name] = entity;
    m_scene->addActor(*entity->actor());
}

void titian::Scene::remove_entity(const String& name)
{
    if (m_entities.contains(name)) {
        m_scene->removeActor(*m_entities.at(name)->actor());
        m_entities.erase(name);
    }
}

bool titian::Scene::contains_entity(const String& name) const
{
    return m_entities.contains(name);
}

size_t titian::Scene::entity_count() const
{
    return m_entities.size();
}

titian::Map<titian::String, titian::Ref<titian::Entity>>::iterator titian::Scene::begin()
{
    return m_entities.begin();
}

titian::Map<titian::String, titian::Ref<titian::Entity>>::iterator titian::Scene::end()
{
    return m_entities.end();
}

titian::Map<titian::String, titian::Ref<titian::Entity>>::const_iterator titian::Scene::begin() const
{
    return m_entities.begin();
}

titian::Map<titian::String, titian::Ref<titian::Entity>>::const_iterator titian::Scene::end() const
{
    return m_entities.end();
}

// Get types
titian::Ref<titian::Mesh> titian::Scene::get_mesh(const String& id) const
{
    if (meshes.contains(id)) {
        return meshes.at(id);
    }
    return nullptr;
}

titian::Ref<titian::Animation> titian::Scene::get_animation(const String& id) const
{
    if (animations.contains(id)) {
        return animations.at(id);
    }
    return nullptr;
}

titian::Ref<titian::Texture> titian::Scene::get_texture(const String& id) const
{
    if (textures.contains(id)) {
        return textures.at(id);
    }
    return nullptr;
}

titian::Ref<titian::Material> titian::Scene::get_material(const String& id) const
{
    if (materials.contains(id)) {
        return materials.at(id);
    }
    return nullptr;
}

titian::Ref<titian::Shader> titian::Scene::get_shader(const String& id) const
{
    if (shaders.contains(id)) {
        return shaders.at(id);
    }
    return nullptr;
}

titian::Ref<titian::Script> titian::Scene::get_script(const String& id) const
{
    if (scripts.contains(id)) {
        return scripts.at(id);
    }
    return nullptr;
}

titian::Ref<titian::Entity> titian::Scene::get_entity(const String& id) const
{
    if (m_entities.contains(id)) {
        return m_entities.at(id);
    }
    return nullptr;
}

// Dynamic colliders
titian::Ref<titian::Collider> titian::Scene::new_box_collider(const Float3& scale) const
{
    return new Collider(m_physics, px::PxBoxGeometry(reinterpret_cast<const px::PxVec3&>(scale)));
}

titian::Ref<titian::Collider> titian::Scene::new_sphere_collider(const float radius) const
{
    return new Collider(m_physics, px::PxSphereGeometry(radius));
}

titian::Ref<titian::Collider> titian::Scene::new_capsule_collider(const float radius, const float height) const
{
    return new Collider(m_physics, px::PxCapsuleGeometry(radius, height));
}

// Static colliders
titian::Ref<titian::Collider> titian::Scene::new_mesh_collider(const Mesh& mesh, const Float3& scale) const
{
    if (mesh.physics_buffer) {
        return new Collider(m_physics, px::PxTriangleMeshGeometry(mesh.physics_buffer, reinterpret_cast<const px::PxVec3&>(scale)));
    }
    return nullptr;
}

// Default collider
titian::Ref<titian::Collider> titian::Scene::new_default_collider(const px::PxGeometryType::Enum type, const Mesh* optional_mesh) const
{
    switch (type) {
        // Dynamic
    case px::PxGeometryType::Enum::eBOX:
        return new_box_collider(Float3{ 1.0f });
    case px::PxGeometryType::Enum::eSPHERE:
        return new_sphere_collider(1.0f);
    case px::PxGeometryType::Enum::eCAPSULE:
        return new_capsule_collider(1.0f, 2.0f);

        // Static
    case px::PxGeometryType::Enum::eTRIANGLEMESH:
        if (optional_mesh) {
            return new_mesh_collider(*optional_mesh, Float3{ 1.0f });
        }
    }
    return nullptr;
}

// Helper new
titian::Mesh* titian::Scene::helper_new_mesh(const String& id)
{
    Mesh* mesh = new Mesh(m_gpu, m_physics, m_cooking);
    meshes[id] = mesh;
    return mesh;
}

titian::Animation* titian::Scene::helper_new_animation(const String& id)
{
    Animation* animation = new Animation(m_gpu, this);
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
    Shader* shader = new Shader(ShaderType::MATERIAL, m_gpu);
    shaders[id] = shader;
    return shader;
}

titian::Entity* titian::Scene::helper_new_entity(const String& id)
{
    Ref entity = this->new_entity(true);
    this->add_entity(id, entity);
    return &entity;
}

// Helper get
titian::Mesh* titian::Scene::helper_get_mesh(const String& id)
{
    if (meshes.contains(id)) {
        return &meshes.at(id);
    }
    return nullptr;
}

titian::Animation* titian::Scene::helper_get_animation(const String& id)
{
    if (animations.contains(id)) {
        return &animations.at(id);
    }
    return nullptr;
}

titian::Texture* titian::Scene::helper_get_texture(const String& id)
{
    if (textures.contains(id)) {
        return &textures.at(id);
    }
    return nullptr;
}

titian::Material* titian::Scene::helper_get_material(const String& id)
{
    if (materials.contains(id)) {
        return &materials.at(id);
    }
    return nullptr;
}

titian::Shader* titian::Scene::helper_get_shader(const String& id)
{
    if (shaders.contains(id)) {
        return &shaders.at(id);
    }
    return nullptr;
}

titian::Entity* titian::Scene::helper_get_entity(const String& id)
{
    return &this->get_entity(id);
}

// Helper remove
void titian::Scene::helper_remove_mesh(const String& id)
{
    meshes.erase(id);
}

void titian::Scene::helper_remove_animation(const String& id)
{
    animations.erase(id);
}

void titian::Scene::helper_remove_texture(const String& id)
{
    textures.erase(id);
}

void titian::Scene::helper_remove_material(const String& id)
{
    materials.erase(id);
}

void titian::Scene::helper_remove_shader(const String& id)
{
    shaders.erase(id);
}

void titian::Scene::helper_remove_entity(const String& id)
{
    this->remove_entity(id);
}

// Helper contains
bool titian::Scene::helper_contains_mesh(const String& id) const
{
    return meshes.contains(id);
}

bool titian::Scene::helper_contains_animation(const String& id) const
{
    return animations.contains(id);
}

bool titian::Scene::helper_contains_texture(const String& id) const
{
    return textures.contains(id);
}

bool titian::Scene::helper_contains_material(const String& id) const
{
    return materials.contains(id);
}

bool titian::Scene::helper_contains_shader(const String& id) const
{
    return shaders.contains(id);
}

bool titian::Scene::helper_contains_entity(const String& id) const
{
    return this->contains_entity(id);
}

// Helper count
int titian::Scene::helper_mesh_count() const
{
    return static_cast<int>(meshes.size());
}

int titian::Scene::helper_animation_count() const
{
    return static_cast<int>(animations.size());
}

int titian::Scene::helper_texture_count() const
{
    return static_cast<int>(textures.size());
}

int titian::Scene::helper_material_count() const
{
    return static_cast<int>(materials.size());
}

int titian::Scene::helper_shader_count() const
{
    return static_cast<int>(shaders.size());
}

int titian::Scene::helper_entity_count() const
{
    return static_cast<int>(m_entities.size());
}

// Helper get all
titian::Map<titian::String, titian::Mesh*> titian::Scene::helper_get_all_meshes()
{
    Map<String, Mesh*> result = {};
    for (auto& [name, mesh] : meshes) {
        result[name] = &mesh;
    }
    return result;
}

titian::Map<titian::String, titian::Animation*> titian::Scene::helper_get_all_animations()
{
    Map<String, Animation*> result = {};
    for (auto& [name, animation] : animations) {
        result[name] = &animation;
    }
    return result;
}

titian::Map<titian::String, titian::Texture*> titian::Scene::helper_get_all_textures()
{
    Map<String, Texture*> result = {};
    for (auto& [name, texture] : textures) {
        result[name] = &texture;
    }
    return result;
}

titian::Map<titian::String, titian::Material*> titian::Scene::helper_get_all_materials()
{
    Map<String, Material*> result = {};
    for (auto& [name, material] : materials) {
        result[name] = &material;
    }
    return result;
}

titian::Map<titian::String, titian::Shader*> titian::Scene::helper_get_all_shaders()
{
    Map<String, Shader*> result = {};
    for (auto& [name, shader] : shaders) {
        result[name] = &shader;
    }
    return result;
}

titian::Map<titian::String, titian::Entity*> titian::Scene::helper_get_all_entities()
{
    Map<String, Entity*> result = {};
    for (auto& [name, entity] : m_entities) {
        result[name] = &entity;
    }
    return result;
}

titian::Optional<titian::AssimpData> titian::Scene::get_assimp_data(const String& path) const
{
    Ref importer = new as::Importer();
    const aiScene* scene = importer->ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_MakeLeftHanded);
    if (!scene) {
        return std::nullopt;
    }

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

// Other
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
    Ref mesh_object = new Mesh(m_gpu, m_physics, m_cooking);
    
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

        // bone vertex data
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

        // bone data
        mesh_object->bone_matrices.resize(mesh->mNumBones);
        for (uint32_t i = 0; i < mesh->mNumBones; i++) {
            auto& bone = mesh->mBones[i];
            auto& offset_matrix = mesh_object->bone_matrices[i];
            for (int j = 0; j < 4; j++) {
                memcpy(&offset_matrix(0, j), bone->mOffsetMatrix[j], 4 * sizeof(float));
            }
        }

        // bone nodes
        Function<Ref<SkeletonNode>(const aiNode*)> recur_helper;
        recur_helper = [&](const aiNode* node)
        {
            Ref<SkeletonNode> skeleton_node = new SkeletonNode();
            // bone index
            skeleton_node->bone_index = -1;
            for (uint32_t i = 0; i < mesh->mNumBones; i++) {
				if (mesh->mBones[i]->mName == node->mName) {
                    skeleton_node->bone_index = i;
					break;
				}
            }

            // transform
            for (int i = 0; i < 4; i++) {
                memcpy(&skeleton_node->transformation(0, i), node->mTransformation[i], 4 * sizeof(float));
            }

            // children
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
    Ref animation_object = new Animation(m_gpu, this);

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

    // animation nodes
    Function<Ref<AnimationNode>(const aiNode*)> recur_helper;
    recur_helper = [&](const aiNode* node)
    {
        Ref<AnimationNode> animation_node = new AnimationNode();
        // channel index
        animation_node->channel_index = -1;
        for (uint32_t i = 0; i < animation->mNumChannels; i++) {
            if (animation->mChannels[i]->mNodeName == node->mName) {
                animation_node->channel_index = i;
                break;
            }
        }

        // children
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
        memcpy(texture_object->data_buffer, texture->pcData, (size_t) texture->mWidth * texture->mHeight * 4);
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
