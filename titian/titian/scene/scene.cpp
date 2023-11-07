#include "main.h"


physx::PxDefaultAllocator     titian::Scene::m_allocator = {};
physx::PxDefaultErrorCallback titian::Scene::m_error_callback = {};
physx::PxFoundation*          titian::Scene::m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);

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
    scene_descriptor.filterShader = physx::PxDefaultSimulationFilterShader;

    m_scene = m_physics->createScene(scene_descriptor);
    kl::assert(m_scene, "Failed to create physics scene");

    default_meshes = new DefaultMeshes(gpu, m_physics, m_cooking);
    kl::assert(default_meshes, "Failed to init default meshes");

    default_materials = new DefaultMaterials();
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

void titian::Scene::serialize(Serializer* serializer, const void* helper_data) const
{
    serializer->write_string(main_camera_name);
    serializer->write_string(main_ambient_light_name);
    serializer->write_string(main_directional_light_name);

    auto write_map = [&]<typename T>(const std::map<std::string, kl::Object<T>>&data, const void* helper_data)
    {
        serializer->write_object<uint64_t>(data.size());
        for (auto& [name, object] : data) {
            serializer->write_string(name);
            object->serialize(serializer, helper_data);
        }
    };

    write_map(meshes, nullptr);
    write_map(textures, nullptr);
    write_map(materials, nullptr);
    write_map(scripts, nullptr);
    write_map(m_entities, &meshes);
}

void titian::Scene::deserialize(const Serializer* serializer, const void* helper_data)
{
    serializer->read_string(main_camera_name);
    serializer->read_string(main_ambient_light_name);
    serializer->read_string(main_directional_light_name);

    auto read_map = [&]<typename T>(std::map<std::string, T>& data, const std::function<T()>& provider, const void* helper_data)
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const std::string name = serializer->read_string();
            kl::Object object = provider();
            object->deserialize(serializer, helper_data);
            data[name] = object;
        }
    };

    std::function mesh_provider = [&] { return kl::Object{ new Mesh(m_gpu, m_physics, m_cooking) }; };
    read_map(meshes, mesh_provider, nullptr);

    std::function texture_provider = [&] { return kl::Object{ new Texture(m_gpu) }; };
    read_map(textures, texture_provider, nullptr);

    std::function material_provider = [&] { return kl::Object{ new Material() }; };
    read_map(materials, material_provider, nullptr);

    /* SCRIPTS */
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const std::string name = serializer->read_string();
            const Script::Type type = serializer->read_object<Script::Type>();

            kl::Object<Script> object = nullptr;
            switch (type) {
            case Script::Type::NATIVE:
                object = new NativeScript();
                break;
            case Script::Type::INTER:
                object = new InterScript();
                break;
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
            const Entity::Type type = serializer->read_object<Entity::Type>();

            kl::Object<Entity> entity = nullptr;
            switch (type) {
            case Entity::Type::BASIC:
                entity = new Entity(Entity::Type::BASIC, m_physics, false);
                break;
            case Entity::Type::CAMERA:
                entity = new Camera(m_physics, false);
                break;
            case Entity::Type::AMBIENT_LIGHT:
                entity = new AmbientLight(m_physics, false);
                break;
            case Entity::Type::POINT_LIGHT:
                entity = new PointLight(m_physics, false);
                break;
            case Entity::Type::DIRECTIONAL_LIGHT:
                entity = new DirectionalLight(m_physics, false, m_gpu, 4096);
                break;
            }

            entity->deserialize(serializer, &meshes);
            this->add_entity(name, entity);
        }
    }
}

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
        script->call_update();
    }
}

// Entity
kl::Object<titian::Entity> titian::Scene::new_entity(const bool dynamic) const
{
    return new Entity(Entity::Type::BASIC, m_physics, dynamic);
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
        return new_mesh_collider(*optional_mesh, kl::Float3{ 1.0f });
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

titian::Entity* titian::Scene::helper_get_entity(const std::string& id)
{
    return &this->get_entity(id);
}

// Helper remove
void titian::Scene::helper_remove_mesh(const std::string& id)
{
    meshes.erase(id);
}

void titian::Scene::helper_remove_texture(const std::string& id)
{
    textures.erase(id);
}

void titian::Scene::helper_remove_material(const std::string& id)
{
    materials.erase(id);
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

bool titian::Scene::helper_contains_texture(const std::string& id) const
{
    return textures.contains(id);
}

bool titian::Scene::helper_contains_material(const std::string& id) const
{
    return materials.contains(id);
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

int titian::Scene::helper_texture_count() const
{
    return static_cast<int>(textures.size());
}

int titian::Scene::helper_material_count() const
{
    return static_cast<int>(materials.size());
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

std::map<std::string, titian::Entity*> titian::Scene::helper_get_all_entities()
{
    std::map<std::string, Entity*> result = {};
    for (auto& [name, entity] : m_entities) {
        result[name] = &entity;
    }
    return result;
}
