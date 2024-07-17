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

    default_meshes = new DefaultMeshes(gpu, m_physics, m_cooking);
    kl::assert(default_meshes, "Failed to init default meshes");

    default_animations = new DefaultAnimations(this);
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

    std::function animation_provider = [&] { return new Animation(this); };
    read_map(animations, animation_provider, nullptr);

    std::function texture_provider = [&] { return new Texture(m_gpu); };
    read_map(textures, texture_provider, nullptr);

    std::function material_provider = [&] { return new Material(); };
    read_map(materials, material_provider, nullptr);

    std::function shader_provider = [&] { return new Shader(ShaderType::NONE, m_gpu); };
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
    Animation* animation = new Animation(this);
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
    Shader* shader = new Shader(ShaderType::NONE, m_gpu);
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

// Other
void titian::Scene::load_gltf(const tinygltf::Model& model, const bool flip_z, const bool flip_v)
{
    auto get_buffer_data = [&]<typename T>(const tinygltf::Accessor& accessor, std::vector<T>& out_data)
    {
        const tinygltf::BufferView& buffer_view = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[buffer_view.buffer];
        const T* data_start = reinterpret_cast<const T*>(buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset);
        const size_t count = accessor.count * tinygltf::GetNumComponentsInType(accessor.type);
        out_data.insert(out_data.end(), data_start, data_start + count);
    };

    for (const auto& mesh : model.meshes) {
        kl::Object mesh_obj = new Mesh(m_gpu, m_physics, m_cooking);
		for (const auto& primitive : mesh.primitives) {
            std::vector<float> positions;
            if (primitive.attributes.contains("POSITION")) {
                const tinygltf::Accessor& pos_accessor = model.accessors[primitive.attributes.at("POSITION")];
                get_buffer_data(pos_accessor, positions);
            }

            std::vector<float> normals;
            if (primitive.attributes.contains("NORMAL")) {
				const tinygltf::Accessor& norm_accessor = model.accessors[primitive.attributes.at("NORMAL")];
				get_buffer_data(norm_accessor, normals);
			}

            std::vector<float> texcoords;
			if (primitive.attributes.contains("TEXCOORD_0")) {
				const tinygltf::Accessor& texcoord_accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
				get_buffer_data(texcoord_accessor, texcoords);
			}

            std::vector<uint32_t> bone_indices;
			if (primitive.attributes.contains("JOINTS_0")) {
                const tinygltf::Accessor& joint_accessor = model.accessors[primitive.attributes.at("JOINTS_0")];
                if (joint_accessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE || joint_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                    std::vector<uint8_t> temp_bone_indices;
                    get_buffer_data(joint_accessor, temp_bone_indices);
                    bone_indices.insert(bone_indices.end(), temp_bone_indices.begin(), temp_bone_indices.end());
                }
                else if (joint_accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT || joint_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                    std::vector<uint16_t> temp_bone_indices;
                    get_buffer_data(joint_accessor, temp_bone_indices);
                    bone_indices.insert(bone_indices.end(), temp_bone_indices.begin(), temp_bone_indices.end());
                }
                else {
                    get_buffer_data(joint_accessor, bone_indices);
                }
			}

            std::vector<float> bone_weights;
			if (primitive.attributes.contains("WEIGHTS_0")) {
				const tinygltf::Accessor& weight_accessor = model.accessors[primitive.attributes.at("WEIGHTS_0")];
				get_buffer_data(weight_accessor, bone_weights);
			}

            std::vector<Vertex> vertices(positions.size() / 3);
            for (int i = 0; i < (int) vertices.size(); i++) {
                Vertex& vertex = vertices[i];
                for (int j = 0; j < 3; j++) {
                    const int index = i * 3 + j;
                    if (index < (int) positions.size()) {
                        vertex.world[j] = positions[index];
                    }
                    if (index < (int) normals.size()) {
                        vertex.normal[j] = normals[index];
                    }
                }
                for (int j = 0; j < 2; j++) {
                    const int index = i * 2 + j;
                    if (index < (int) texcoords.size()) {
                        vertex.texture[j] = texcoords[index];
                    }
                }
                for (int j = 0; j < 4; j++) {
					const int index = i * 4 + j;
                    if (index < (int) bone_indices.size()) {
                        vertex.bone_indices[j] = (uint8_t) bone_indices[index];
                    }
                    if (index < (int) bone_weights.size()) {
                        vertex.bone_weights[j] = bone_weights[index];
                    }
                }
                if (flip_z) {
                    vertex.world.z *= -1.0f;
                    vertex.normal.z *= -1.0f;
                }
                if (flip_v) {
                    vertex.texture.y = 1.0f - vertex.texture.y;
                }
            }

            std::vector<uint32_t> indices;
            if (primitive.indices > -1) {
                const tinygltf::Accessor& index_accessor = model.accessors[primitive.indices];
                if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE || index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
					std::vector<uint8_t> temp_indices;
					get_buffer_data(index_accessor, temp_indices);
					indices.insert(indices.end(), temp_indices.begin(), temp_indices.end());
				}
				else if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT || index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
					std::vector<uint16_t> temp_indices;
					get_buffer_data(index_accessor, temp_indices);
					indices.insert(indices.end(), temp_indices.begin(), temp_indices.end());
				}
				else {
					get_buffer_data(index_accessor, indices);
				}
            }

            mesh_obj->data_buffer.resize(indices.size());
            for (uint64_t i = 0; i < indices.size(); i++) {
                mesh_obj->data_buffer[i] = vertices[indices[i]];
            }
        }
        mesh_obj->reload();

        const std::string name_templ = !mesh.name.empty() ? mesh.name : "unknown_mesh";
        std::string name = name_templ;
        for (int i = 0; meshes.contains(name); i++) {
            name = kl::format(name_templ, '_', i);
        }
        meshes[name] = mesh_obj;
    }

    for (const auto& animation : model.animations) {
        kl::Object animation_obj = new Animation(this);

		for (const auto& channel : animation.channels) {

		}
		for (const auto& sampler : animation.samplers) {

		}

		const std::string name_templ = !animation.name.empty() ? animation.name : "unknown_animation";
		std::string name = name_templ;
		for (int i = 0; animations.contains(name); i++) {
			name = kl::format(name_templ, '_', i);
		}
		animations[name] = animation_obj;
    }

    for (const auto& texture : model.textures) {
        const int image_index = texture.source;
        if (image_index < 0 || image_index >= model.images.size()) {
            continue;
        }

        const tinygltf::Image& image_data = model.images[image_index];
        if (image_data.image.empty()) {
            continue;
        }

        kl::Object texture_obj = new Texture(m_gpu);
        texture_obj->data_buffer.resize({ image_data.width, image_data.height });
        kl::Color* color_data = texture_obj->data_buffer;
        for (size_t i = 0; i < texture_obj->data_buffer.pixel_count(); ++i) {
            color_data[i].r = image_data.image[i * 4 + 0];
            color_data[i].g = image_data.image[i * 4 + 1];
            color_data[i].b = image_data.image[i * 4 + 2];
            color_data[i].a = image_data.image[i * 4 + 3];
        }
        texture_obj->reload_as_2D();
        texture_obj->create_shader_view();

		const std::string name_templ = !texture.name.empty() ? texture.name : "unknown_texture";
		std::string name = name_templ;
		for (int i = 0; textures.contains(name); i++) {
			name = kl::format(name_templ, '_', i);
		}
		textures[name] = texture_obj;
    }

    for (const auto& material : model.materials) {
        kl::Object material_obj = new Material();
        material_obj->color = {
            (float) material.pbrMetallicRoughness.baseColorFactor[0],
            (float) material.pbrMetallicRoughness.baseColorFactor[1],
            (float) material.pbrMetallicRoughness.baseColorFactor[2],
            (float) material.pbrMetallicRoughness.baseColorFactor[3],
        };
		material_obj->reflection_factor = 1.0f - (float) material.pbrMetallicRoughness.roughnessFactor;
        if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
            material_obj->color_map_name = model.textures[material.pbrMetallicRoughness.baseColorTexture.index].name;
        }
        if (material.normalTexture.index >= 0) {
            material_obj->normal_map_name = model.textures[material.normalTexture.index].name;
        }

        const std::string name_templ = !material.name.empty() ? material.name : "unknown_material";
        std::string name = name_templ;
        for (int i = 0; materials.contains(name); i++) {
            name = kl::format(name_templ, '_', i);
        }
        materials[name] = material_obj;
    }
}
