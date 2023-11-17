﻿#pragma once

#include "components/mesh.h"
#include "components/texture.h"
#include "components/material.h"
#include "scripting/script.h"
#include "scene/entity.h"

#include "scene/default_meshes.h"
#include "scene/default_materials.h"


namespace titian {
    class Scene : public Serializable
    {
    public:
        std::map<std::string, kl::Object<Mesh>> meshes = {};
        std::map<std::string, kl::Object<Texture>> textures = {};
        std::map<std::string, kl::Object<Material>> materials = {};
        std::map<std::string, kl::Object<Script>> scripts = {};

        kl::Object<DefaultMeshes> default_meshes = nullptr;
        kl::Object<DefaultMaterials> default_materials = nullptr;

        std::string main_camera_name = "/";
        std::string main_ambient_light_name = "/";
        std::string main_directional_light_name = "/";

        Scene(kl::GPU* gpu);
        ~Scene() override;

        Scene(const Scene&) = delete;
        Scene(const Scene&&) = delete;

        void operator=(const Scene&) = delete;
        void operator=(const Scene&&) = delete;

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        // Get ptrs
        physx::PxPhysics* physics() const;
        physx::PxCooking* cooking() const;

        // Gravity
        void set_gravity(const kl::Float3& gravity);
        kl::Float3 gravity() const;

        // Update
        void update_physics(float delta_t);
        void update_scripts();

        // Entity
        kl::Object<Entity> new_entity(bool dynamic) const;
        void add_entity(const std::string& name, const kl::Object<Entity>& entity);
        void remove_entity(const std::string& name);
        bool contains_entity(const std::string& name) const;
        size_t entity_count() const;

        std::map<std::string, kl::Object<Entity>>::iterator begin();
        std::map<std::string, kl::Object<Entity>>::iterator end();
        std::map<std::string, kl::Object<Entity>>::const_iterator begin() const;
        std::map<std::string, kl::Object<Entity>>::const_iterator end() const;

        // Get types
        kl::Object<Mesh> get_mesh(const std::string& id) const;
        kl::Object<Texture> get_texture(const std::string& id) const;
        kl::Object<Material> get_material(const std::string& id) const;
        kl::Object<Script> get_script(const std::string& id) const;
        kl::Object<Entity> get_entity(const std::string& id) const;

        // Casted
        template<typename T, typename... Args>
        kl::Object<T> new_casted(const bool dynamic, const Args&... args) const
        {
            return new T(m_physics, dynamic, args...);
        }

        template<typename T>
        T* get_casted(const std::string& id)
        {
            Entity* entity = &get_entity(id);
            return dynamic_cast<T*>(entity);
        }

        template<typename T>
        const T* get_casted(const std::string& id) const
        {
            const Entity* entity = &get_entity(id);
            return dynamic_cast<const T*>(entity);
        }

        // Dynamic colliders
        kl::Object<Collider> new_box_collider(const kl::Float3& scale) const;
        kl::Object<Collider> new_sphere_collider(float radius) const;
        kl::Object<Collider> new_capsule_collider(float radius, float height) const;

        // Static colliders
        kl::Object<Collider> new_mesh_collider(const Mesh& mesh, const kl::Float3& scale) const;

        // Default collider
        kl::Object<Collider> new_default_collider(physx::PxGeometryType::Enum type, const Mesh* optional_mesh) const;

        // Helper new
        Mesh* helper_new_mesh(const std::string& id);
        Texture* helper_new_texture(const std::string& id);
        Material* helper_new_material(const std::string& id);
        Entity* helper_new_entity(const std::string& id);

        // Helper get
        Mesh* helper_get_mesh(const std::string& id);
        Texture* helper_get_texture(const std::string& id);
        Material* helper_get_material(const std::string& id);
        Entity* helper_get_entity(const std::string& id);

        // Helper remove
        void helper_remove_mesh(const std::string& id);
        void helper_remove_texture(const std::string& id);
        void helper_remove_material(const std::string& id);
        void helper_remove_entity(const std::string& id);

        // Helper contains
        bool helper_contains_mesh(const std::string& id) const;
        bool helper_contains_texture(const std::string& id) const;
        bool helper_contains_material(const std::string& id) const;
        bool helper_contains_entity(const std::string& id) const;

        // Helper count
        int helper_mesh_count() const;
        int helper_texture_count() const;
        int helper_material_count() const;
        int helper_entity_count() const;

        // Helper get all
        std::map<std::string, Mesh*> helper_get_all_meshes();
        std::map<std::string, Texture*> helper_get_all_textures();
        std::map<std::string, Material*> helper_get_all_materials();
        std::map<std::string, Entity*> helper_get_all_entities();

    private:
        static physx::PxDefaultAllocator m_allocator;
        static physx::PxDefaultErrorCallback m_error_callback;
        static physx::PxFoundation* m_foundation;

        physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
        physx::PxPhysics* m_physics = nullptr;
        physx::PxCooking* m_cooking = nullptr;
        physx::PxScene* m_scene = nullptr;

        kl::GPU* m_gpu = nullptr;
        std::map<std::string, kl::Object<Entity>> m_entities = {};
    };
}