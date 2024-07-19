#include "main.h"


physx::PxDefaultAllocator     titian::Scene::m_allocator = {};
physx::PxDefaultErrorCallback titian::Scene::m_error_callback = {};
physx::PxFoundation*          titian::Scene::m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);

static physx::PxFilterFlags filter_shader(
    physx::PxFilterObjectAttributes attributes0,
    physx::PxFilterData filterData0,
    physx::PxFilterObjectAttributes attributes1,
    physx::PxFilterData filterData1,
    physx::PxPairFlags& pairFlags,
    const void* constantBlock,
    physx::PxU32 constantBlockSize)
{
    pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
    pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
    return physx::PxFilterFlag::eDEFAULT;
}

titian::Scene::Scene(kl::GPU* gpu)
    : m_gpu(gpu)
{
    m_dispatcher = physx::PxDefaultCpuDispatcherCreate(kl::CPU_CORE_COUNT / 4);
    kl::assert(m_dispatcher, "Failed to create physics dispatcher");

    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale());
    kl::assert(m_physics, "Failed to create physics");

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, physx::PxCookingParams(m_physics->getTolerancesScale()));
    kl::assert(m_cooking, "Failed to create physics cooking");

    physx::PxSceneDesc scene_descriptor = { m_physics->getTolerancesScale() };
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

    auto write_map = [&]<typename T>(const std::map<std::string, kl::Object<T>>& data, const void* helper_data)
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

    auto read_map = [&]<typename T>(std::map<std::string, kl::Object<T>>& data, const std::function<T*()>& provider, const void* helper_data)
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const std::string name = serializer->read_string();
            kl::Object<T> object = provider();
            object->deserialize(serializer, helper_data);
            data[name] = object;
        }
    };

    std::function mesh_provider = [&] { return new Mesh(m_gpu, m_physics, m_cooking); };
    read_map(meshes, mesh_provider, nullptr);

    std::function animation_provider = [&] { return new Animation(m_gpu, this); };
    read_map(animations, animation_provider, nullptr);

    std::function texture_provider = [&] { return new Texture(m_gpu); };
    read_map(textures, texture_provider, nullptr);

    std::function material_provider = [&] { return new Material(); };
    read_map(materials, material_provider, nullptr);

    std::function shader_provider = [&] { return new Shader(ShaderType::MATERIAL, m_gpu); };
    read_map(shaders, shader_provider, nullptr);

    /* SCRIPTS */
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const std::string name = serializer->read_string();
            const ScriptType type = serializer->read_object<ScriptType>();

            kl::Object<Script> object = nullptr;
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
            const std::string name = serializer->read_string();
            const EntityType type = serializer->read_object<EntityType>();

            kl::Object<Entity> entity = nullptr;
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

void titian::Scene::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{}

void titian::Scene::onWake(physx::PxActor** actors, physx::PxU32 count)
{}

void titian::Scene::onSleep(physx::PxActor** actors, physx::PxU32 count)
{}

void titian::Scene::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
    Entity* entity_0 = static_cast<Entity*>(pairHeader.actors[0]->userData);
    Entity* entity_1 = static_cast<Entity*>(pairHeader.actors[1]->userData);
    if (entity_0 && entity_1) {
        for (auto& [_, script] : scripts) {
            script->call_collision(this, entity_0, entity_1);
        }
    }
}

void titian::Scene::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{}

void titian::Scene::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{}

// Get ptrs
physx::PxPhysics* titian::Scene::physics() const
{
    return m_physics;
}

physx::PxCooking* titian::Scene::cooking() const
{
    return m_cooking;
}

// Gravity
void titian::Scene::set_gravity(const kl::Float3& gravity)
{
    m_scene->setGravity(reinterpret_cast<const physx::PxVec3&>(gravity));
}

kl::Float3 titian::Scene::gravity() const
{
    const physx::PxVec3 gravity = m_scene->getGravity();
    return reinterpret_cast<const kl::Float3&>(gravity);
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
kl::Object<titian::Entity> titian::Scene::new_entity(const bool dynamic) const
{
    return new Entity(EntityType::BASIC, m_physics, dynamic);
}

void titian::Scene::add_entity(const std::string& name, const kl::Object<Entity>& entity)
{
    m_entities[name] = entity;
    m_scene->addActor(*entity->actor());
}

void titian::Scene::remove_entity(const std::string& name)
{
    if (m_entities.contains(name)) {
        m_scene->removeActor(*m_entities.at(name)->actor());
        m_entities.erase(name);
    }
}

bool titian::Scene::contains_entity(const std::string& name) const
{
    return m_entities.contains(name);
}

size_t titian::Scene::entity_count() const
{
    return m_entities.size();
}

std::map<std::string, kl::Object<titian::Entity>>::iterator titian::Scene::begin()
{
    return m_entities.begin();
}

std::map<std::string, kl::Object<titian::Entity>>::iterator titian::Scene::end()
{
    return m_entities.end();
}

std::map<std::string, kl::Object<titian::Entity>>::const_iterator titian::Scene::begin() const
{
    return m_entities.begin();
}

std::map<std::string, kl::Object<titian::Entity>>::const_iterator titian::Scene::end() const
{
    return m_entities.end();
}

// Get types
kl::Object<titian::Mesh> titian::Scene::get_mesh(const std::string& id) const
{
    if (meshes.contains(id)) {
        return meshes.at(id);
    }
    return nullptr;
}

kl::Object<titian::Animation> titian::Scene::get_animation(const std::string& id) const
{
    if (animations.contains(id)) {
        return animations.at(id);
    }
    return nullptr;
}

kl::Object<titian::Texture> titian::Scene::get_texture(const std::string& id) const
{
    if (textures.contains(id)) {
        return textures.at(id);
    }
    return nullptr;
}

kl::Object<titian::Material> titian::Scene::get_material(const std::string& id) const
{
    if (materials.contains(id)) {
        return materials.at(id);
    }
    return nullptr;
}

kl::Object<titian::Shader> titian::Scene::get_shader(const std::string& id) const
{
    if (shaders.contains(id)) {
        return shaders.at(id);
    }
    return nullptr;
}

kl::Object<titian::Script> titian::Scene::get_script(const std::string& id) const
{
    if (scripts.contains(id)) {
        return scripts.at(id);
    }
    return nullptr;
}

kl::Object<titian::Entity> titian::Scene::get_entity(const std::string& id) const
{
    if (m_entities.contains(id)) {
        return m_entities.at(id);
    }
    return nullptr;
}

// Dynamic colliders
kl::Object<titian::Collider> titian::Scene::new_box_collider(const kl::Float3& scale) const
{
    return new Collider(m_physics, physx::PxBoxGeometry(reinterpret_cast<const physx::PxVec3&>(scale)));
}

kl::Object<titian::Collider> titian::Scene::new_sphere_collider(const float radius) const
{
    return new Collider(m_physics, physx::PxSphereGeometry(radius));
}

kl::Object<titian::Collider> titian::Scene::new_capsule_collider(const float radius, const float height) const
{
    return new Collider(m_physics, physx::PxCapsuleGeometry(radius, height));
}

// Static colliders
kl::Object<titian::Collider> titian::Scene::new_mesh_collider(const Mesh& mesh, const kl::Float3& scale) const
{
    if (mesh.physics_buffer) {
        return new Collider(m_physics, physx::PxTriangleMeshGeometry(mesh.physics_buffer, reinterpret_cast<const physx::PxVec3&>(scale)));
    }
    return nullptr;
}

// Default collider
kl::Object<titian::Collider> titian::Scene::new_default_collider(const physx::PxGeometryType::Enum type, const Mesh* optional_mesh) const
{
    switch (type) {
        // Dynamic
    case physx::PxGeometryType::Enum::eBOX:
        return new_box_collider(kl::Float3{ 1.0f });
    case physx::PxGeometryType::Enum::eSPHERE:
        return new_sphere_collider(1.0f);
    case physx::PxGeometryType::Enum::eCAPSULE:
        return new_capsule_collider(1.0f, 2.0f);

        // Static
    case physx::PxGeometryType::Enum::eTRIANGLEMESH:
        if (optional_mesh) {
            return new_mesh_collider(*optional_mesh, kl::Float3{ 1.0f });
        }
    }
    return nullptr;
}

// Helper new
titian::Mesh* titian::Scene::helper_new_mesh(const std::string& id)
{
    Mesh* mesh = new Mesh(m_gpu, m_physics, m_cooking);
    meshes[id] = mesh;
    return mesh;
}

titian::Animation* titian::Scene::helper_new_animation(const std::string& id)
{
    Animation* animation = new Animation(m_gpu, this);
    animations[id] = animation;
    return animation;
}

titian::Texture* titian::Scene::helper_new_texture(const std::string& id)
{
    Texture* texture = new Texture(m_gpu);
    textures[id] = texture;
    return texture;
}

titian::Material* titian::Scene::helper_new_material(const std::string& id)
{
    Material* material = new Material();
    materials[id] = material;
    return material;
}

titian::Shader* titian::Scene::helper_new_shader(const std::string& id)
{
    Shader* shader = new Shader(ShaderType::MATERIAL, m_gpu);
    shaders[id] = shader;
    return shader;
}

titian::Entity* titian::Scene::helper_new_entity(const std::string& id)
{
    kl::Object entity = this->new_entity(true);
    this->add_entity(id, entity);
    return &entity;
}

// Helper get
titian::Mesh* titian::Scene::helper_get_mesh(const std::string& id)
{
    if (meshes.contains(id)) {
        return &meshes.at(id);
    }
    return nullptr;
}

titian::Animation* titian::Scene::helper_get_animation(const std::string& id)
{
    if (animations.contains(id)) {
        return &animations.at(id);
    }
    return nullptr;
}

titian::Texture* titian::Scene::helper_get_texture(const std::string& id)
{
    if (textures.contains(id)) {
        return &textures.at(id);
    }
    return nullptr;
}

titian::Material* titian::Scene::helper_get_material(const std::string& id)
{
    if (materials.contains(id)) {
        return &materials.at(id);
    }
    return nullptr;
}

titian::Shader* titian::Scene::helper_get_shader(const std::string& id)
{
    if (shaders.contains(id)) {
        return &shaders.at(id);
    }
    return nullptr;
}

titian::Entity* titian::Scene::helper_get_entity(const std::string& id)
{
    return &this->get_entity(id);
}

// Helper remove
void titian::Scene::helper_remove_mesh(const std::string& id)
{
    meshes.erase(id);
}

void titian::Scene::helper_remove_animation(const std::string& id)
{
    animations.erase(id);
}

void titian::Scene::helper_remove_texture(const std::string& id)
{
    textures.erase(id);
}

void titian::Scene::helper_remove_material(const std::string& id)
{
    materials.erase(id);
}

void titian::Scene::helper_remove_shader(const std::string& id)
{
    shaders.erase(id);
}

void titian::Scene::helper_remove_entity(const std::string& id)
{
    this->remove_entity(id);
}

// Helper contains
bool titian::Scene::helper_contains_mesh(const std::string& id) const
{
    return meshes.contains(id);
}

bool titian::Scene::helper_contains_animation(const std::string& id) const
{
    return animations.contains(id);
}

bool titian::Scene::helper_contains_texture(const std::string& id) const
{
    return textures.contains(id);
}

bool titian::Scene::helper_contains_material(const std::string& id) const
{
    return materials.contains(id);
}

bool titian::Scene::helper_contains_shader(const std::string& id) const
{
    return shaders.contains(id);
}

bool titian::Scene::helper_contains_entity(const std::string& id) const
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
std::map<std::string, titian::Mesh*> titian::Scene::helper_get_all_meshes()
{
    std::map<std::string, Mesh*> result = {};
    for (auto& [name, mesh] : meshes) {
        result[name] = &mesh;
    }
    return result;
}

std::map<std::string, titian::Animation*> titian::Scene::helper_get_all_animations()
{
    std::map<std::string, Animation*> result = {};
    for (auto& [name, animation] : animations) {
        result[name] = &animation;
    }
    return result;
}

std::map<std::string, titian::Texture*> titian::Scene::helper_get_all_textures()
{
    std::map<std::string, Texture*> result = {};
    for (auto& [name, texture] : textures) {
        result[name] = &texture;
    }
    return result;
}

std::map<std::string, titian::Material*> titian::Scene::helper_get_all_materials()
{
    std::map<std::string, Material*> result = {};
    for (auto& [name, material] : materials) {
        result[name] = &material;
    }
    return result;
}

std::map<std::string, titian::Shader*> titian::Scene::helper_get_all_shaders()
{
    std::map<std::string, Shader*> result = {};
    for (auto& [name, shader] : shaders) {
        result[name] = &shader;
    }
    return result;
}

std::map<std::string, titian::Entity*> titian::Scene::helper_get_all_entities()
{
    std::map<std::string, Entity*> result = {};
    for (auto& [name, entity] : m_entities) {
        result[name] = &entity;
    }
    return result;
}

std::optional<titian::AssimpData> titian::Scene::get_assimp_data(const std::string& path) const
{
    kl::Object importer = new Assimp::Importer();
    const aiScene* scene = importer->ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_MakeLeftHanded);
    if (!scene) {
        return std::nullopt;
    }

    AssimpData data{};
    data.importer = importer;
    for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        std::string name = mesh->mName.C_Str();
        if (name.empty()) {
            name = "unknown_mesh";
        }
        data.meshes.push_back(generate_unique_name(name, meshes));
    }
	for (uint32_t i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation* animation = scene->mAnimations[i];
        std::string name = animation->mName.C_Str();
        if (name.empty()) {
            name = "unknown_animation";
        }
        data.animations.push_back(generate_unique_name(name, animations));
	}
	for (uint32_t i = 0; i < scene->mNumTextures; i++) {
		aiTexture* texture = scene->mTextures[i];
        std::string name = texture->mFilename.C_Str();
		if (name.empty()) {
			name = "unknown_texture";
		}
        data.textures.push_back(generate_unique_name(name, textures));
	}
    for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];
		std::string name = material->GetName().C_Str();
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

kl::Object<titian::Mesh> titian::Scene::load_assimp_mesh(const aiScene* scene, const aiMesh* mesh)
{
    kl::Object mesh_object = new Mesh(m_gpu, m_physics, m_cooking);
    
    std::vector<Vertex> vertices(mesh->mNumVertices);
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
        std::function<kl::Object<SkeletonNode>(const aiNode*)> recur_helper;
        recur_helper = [&](const aiNode* node)
        {
            kl::Object<SkeletonNode> skeleton_node = new SkeletonNode();
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

kl::Object<titian::Animation> titian::Scene::load_assimp_animation(const aiScene* scene, const aiAnimation* animation)
{
    kl::Object animation_object = new Animation(m_gpu, this);

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
    std::function<kl::Object<AnimationNode>(const aiNode*)> recur_helper;
    recur_helper = [&](const aiNode* node)
    {
        kl::Object<AnimationNode> animation_node = new AnimationNode();
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

kl::Object<titian::Texture> titian::Scene::load_assimp_texture(const aiScene* scene, const aiTexture* texture)
{
    kl::Object texture_object = new Texture(m_gpu);

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

kl::Object<titian::Material> titian::Scene::load_assimp_material(const aiScene* scene, const aiMaterial* material)
{
	kl::Object material_object = new Material();

    material->Get(AI_MATKEY_COLOR_DIFFUSE, material_object->color);
	material->Get(AI_MATKEY_COLOR_TRANSPARENT, material_object->color.w);
	material->Get(AI_MATKEY_REFLECTIVITY, material_object->reflection_factor);
	material->Get(AI_MATKEY_REFRACTI, material_object->refraction_index);

	return material_object;
}
