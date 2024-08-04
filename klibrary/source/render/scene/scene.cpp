#include "klibrary.h"


#ifdef KL_USING_PHYSX

// Static
physx::PxDefaultAllocator kl::Scene::m_allocator = {};
physx::PxDefaultErrorCallback kl::Scene::m_error_callback = {};
physx::PxFoundation* kl::Scene::m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);

// Creation
kl::Scene::Scene()
{
    m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    assert(m_dispatcher, "Failed to create physics dispatcher_");
    
    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale());
    assert(m_physics, "Failed to create physics");

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, physx::PxCookingParams(m_physics->getTolerancesScale()));
    assert(m_cooking, "Failed to create physics cooking");

    physx::PxSceneDesc scene_descriptor = { m_physics->getTolerancesScale() };
    scene_descriptor.gravity.y = -9.81f;
    scene_descriptor.cpuDispatcher = m_dispatcher;
    scene_descriptor.filterShader = physx::PxDefaultSimulationFilterShader;

    m_scene = m_physics->createScene(scene_descriptor);
    assert(m_scene, "Failed to create physics scene");
}

kl::Scene::~Scene()
{
    materials.clear();
    textures.clear();
    meshes.clear();

    selected_entity = nullptr;
    while (!m_entities.empty()) {
        this->remove(m_entities.begin()->first);
    }

    m_scene->release();
    m_dispatcher->release();
    m_cooking->release();
    m_physics->release();
}

// Iterate
std::map<std::string, kl::Ref<kl::Entity>>::iterator kl::Scene::begin()
{
    return m_entities.begin();
}

std::map<std::string, kl::Ref<kl::Entity>>::iterator kl::Scene::end()
{
    return m_entities.end();
}

std::map<std::string, kl::Ref<kl::Entity>>::const_iterator kl::Scene::begin() const
{
    return m_entities.begin();
}

std::map<std::string, kl::Ref<kl::Entity>>::const_iterator kl::Scene::end() const
{
    return m_entities.end();
}

// Get
physx::PxPhysics* kl::Scene::physics() const
{
    return m_physics;
}

physx::PxCooking* kl::Scene::cooking() const
{
    return m_cooking;
}

kl::Ref<kl::Entity> kl::Scene::find_entity(const std::string& name) const
{
    if (m_entities.contains(name)) {
        return m_entities.at(name);
    }
    return nullptr;
}

std::string kl::Scene::find_name(const Ref<kl::Entity>& entity) const
{
    for (auto& [name, ent] : m_entities) {
        if (ent == entity) {
            return name;
        }
    }
    return "";
}

size_t kl::Scene::entity_count() const
{
    return m_entities.size();
}

// Set/Get
void kl::Scene::set_gravity(const Float3& gravity)
{
    m_scene->setGravity((const physx::PxVec3&) gravity);
}

kl::Float3 kl::Scene::gravity() const
{
    const physx::PxVec3 gravity = m_scene->getGravity();
    return (const Float3&) gravity;
}

void kl::Scene::add(const std::string& name, const Ref<kl::Entity>& entity)
{
    m_entities[name] = entity;
    m_scene->addActor(*entity->actor());
}

void kl::Scene::remove(const std::string& name)
{
    if (m_entities.contains(name)) {
        m_scene->removeActor(*m_entities.at(name)->actor());
        m_entities.erase(name);
    }
}

// Scene properties
kl::Ref<kl::Entity> kl::Scene::update_selected_entity(const uint32_t index)
{
    if (index != 0) {
        for (auto& [name, entity] : *this) {
            if (entity->unique_index == index) {
                return selected_entity = entity;
            }
        }
    }
    return selected_entity = nullptr;
}

void kl::Scene::update_physics(const float delta_t)
{
    m_scene->simulate(delta_t);
    m_scene->fetchResults(true);
}

// Entity
kl::Ref<kl::Entity> kl::Scene::make_entity(const bool dynamic) const
{
    return new kl::Entity(m_physics, dynamic);
}

// Dynamic colliders
kl::Ref<kl::Collider> kl::Scene::make_box_collider(const Float3& scale) const
{
    return new Collider(m_physics, physx::PxBoxGeometry((physx::PxVec3&) scale));
}

kl::Ref<kl::Collider> kl::Scene::make_sphere_collider(const float radius) const
{
    return new Collider(m_physics, physx::PxSphereGeometry(radius));
}

kl::Ref<kl::Collider> kl::Scene::make_capsule_collider(const float radius, const float height) const
{
    return new Collider(m_physics, physx::PxCapsuleGeometry(radius, height));
}

// Static colliders
kl::Ref<kl::Collider> kl::Scene::make_plane_collider() const
{
    return new Collider(m_physics, physx::PxPlaneGeometry());
}

kl::Ref<kl::Collider> kl::Scene::make_mesh_collider(const Mesh& mesh, const Float3& scale) const
{
    return new Collider(m_physics, physx::PxTriangleMeshGeometry(mesh.physics_buffer, (physx::PxVec3&) scale));
}

// Default collider
kl::Ref<kl::Collider> kl::Scene::make_default_collider(const physx::PxGeometryType::Enum type, const Mesh* optional_mesh) const
{
    switch (type) {
    case physx::PxGeometryType::Enum::eBOX:
        return make_box_collider(Float3 {1.0f});
    case physx::PxGeometryType::Enum::eSPHERE:
        return make_sphere_collider(1.0f);
    case physx::PxGeometryType::Enum::eCAPSULE:
        return make_capsule_collider(1.0f, 2.0f);

    case physx::PxGeometryType::Enum::ePLANE:
        return make_plane_collider();
    case physx::PxGeometryType::Enum::eTRIANGLEMESH:
        return make_mesh_collider(*optional_mesh, Float3 {1.0f});
    }
    return nullptr;
}

#else

void kl::Scene::update_physics(const float delta_t)
{
    for (auto& entity : *this) {
        entity->velocity += gravity * delta_t;
        entity->velocity += entity->acceleration * delta_t;
        entity->position += entity->velocity * delta_t;
        entity->rotation += entity->angular * delta_t;
    }
}

#endif
