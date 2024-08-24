﻿#pragma once

#include "components/mesh.h"
#include "components/animation.h"
#include "components/texture.h"
#include "components/material.h"
#include "components/shader.h"
#include "scripting/script.h"
#include "scene/entity.h"

#include "scene/default/default_meshes.h"
#include "scene/default/default_animations.h"
#include "scene/default/default_materials.h"


namespace titian {
    struct AssimpData
    {
        Ref<as::Importer> importer;
        Vector<String> meshes;
        Vector<String> animations;
        Vector<String> textures;
        Vector<String> materials;
    };
}

namespace titian {
    class Scene : kl::NoCopy, public Serializable, public px::PxSimulationEventCallback
    {
    public:
        StringMap<Ref<Mesh>> meshes;
        StringMap<Ref<Animation>> animations;
        StringMap<Ref<Texture>> textures;
        StringMap<Ref<Material>> materials;
        StringMap<Ref<Shader>> shaders;
        StringMap<Ref<Script>> scripts;

        Ref<DefaultMeshes> default_meshes = nullptr;
        Ref<DefaultAnimations> default_animations = nullptr;
        Ref<DefaultMaterials> default_materials = nullptr;

        String main_camera_name = "/";
        String main_ambient_light_name = "/";
        String main_directional_light_name = "/";

        Scene(kl::GPU* gpu);
        ~Scene() override;

        // Overrides
        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;
        
        void onConstraintBreak(px::PxConstraintInfo* constraints, px::PxU32 count) override;
        void onWake(px::PxActor** actors, px::PxU32 count) override;
        void onSleep(px::PxActor** actors, px::PxU32 count) override;
        void onContact(const px::PxContactPairHeader& pairHeader, const px::PxContactPair* pairs, px::PxU32 nbPairs) override;
        void onTrigger(px::PxTriggerPair* pairs, px::PxU32 count) override;
        void onAdvance(const px::PxRigidBody* const* bodyBuffer, const px::PxTransform* poseBuffer, const px::PxU32 count) override;

        // Get ptrs
        px::PxPhysics* physics() const;
        px::PxCooking* cooking() const;

        // Gravity
        void set_gravity(const Float3& gravity);
        Float3 gravity() const;

        // Update
        void update_physics(float delta_t);
        void update_scripts();
        void update_ui();

        // Entity
        Ref<Entity> new_entity(bool dynamic) const;
        void add_entity(const StringView& name, const Ref<Entity>& entity);
        void remove_entity(const StringView& name);
        bool contains_entity(const StringView& name) const;
        const StringMap<Ref<Entity>>& entities_ref() const;
        size_t entity_count() const;

        StringMap<Ref<Entity>>::iterator begin();
        StringMap<Ref<Entity>>::iterator end();
        StringMap<Ref<Entity>>::const_iterator begin() const;
        StringMap<Ref<Entity>>::const_iterator end() const;

        // Get types
        Ref<Mesh> get_mesh(const StringView& id) const;
        Ref<Animation> get_animation(const StringView& id) const;
        Ref<Texture> get_texture(const StringView& id) const;
        Ref<Material> get_material(const StringView& id) const;
        Ref<Shader> get_shader(const StringView& id) const;
        Ref<Script> get_script(const StringView& id) const;
        Ref<Entity> get_entity(const StringView& id) const;

        // Casted
        template<typename T, typename... Args>
        Ref<T> new_casted(const bool dynamic, const Args&... args) const
        {
            return new T(m_physics, dynamic, args...);
        }

        template<typename T>
        T* get_casted(const StringView& id)
        {
            Entity* entity = &get_entity(id);
            return dynamic_cast<T*>(entity);
        }

        template<typename T>
        const T* get_casted(const StringView& id) const
        {
            const Entity* entity = &get_entity(id);
            return dynamic_cast<const T*>(entity);
        }

        // Dynamic colliders
        Ref<Collider> new_box_collider(const Float3& scale) const;
        Ref<Collider> new_sphere_collider(float radius) const;
        Ref<Collider> new_capsule_collider(float radius, float height) const;

        // Static colliders
        Ref<Collider> new_mesh_collider(const Mesh& mesh, const Float3& scale) const;

        // Default collider
        Ref<Collider> new_default_collider(px::PxGeometryType::Enum type, const Mesh* optional_mesh) const;

        // Helper new
        Mesh* helper_new_mesh(const StringView& id);
        Animation* helper_new_animation(const StringView& id);
        Texture* helper_new_texture(const StringView& id);
        Material* helper_new_material(const StringView& id);
        Shader* helper_new_shader(const StringView& id);
        Entity* helper_new_entity(const StringView& id);

        // Helper get
        Mesh* helper_get_mesh(const StringView& id);
        Animation* helper_get_animation(const StringView& id);
        Texture* helper_get_texture(const StringView& id);
        Material* helper_get_material(const StringView& id);
        Shader* helper_get_shader(const StringView& id);
        Entity* helper_get_entity(const StringView& id);

        // Helper remove
        void helper_remove_mesh(const StringView& id);
        void helper_remove_animation(const StringView& id);
        void helper_remove_texture(const StringView& id);
        void helper_remove_material(const StringView& id);
        void helper_remove_shader(const StringView& id);
        void helper_remove_entity(const StringView& id);

        // Helper contains
        bool helper_contains_mesh(const StringView& id) const;
        bool helper_contains_animation(const StringView& id) const;
        bool helper_contains_texture(const StringView& id) const;
        bool helper_contains_material(const StringView& id) const;
        bool helper_contains_shader(const StringView& id) const;
        bool helper_contains_entity(const StringView& id) const;

        // Helper count
        int helper_mesh_count() const;
        int helper_animation_count() const;
        int helper_texture_count() const;
        int helper_material_count() const;
        int helper_shader_count() const;
        int helper_entity_count() const;

        // Helper get all
        StringMap<Mesh*> helper_get_all_meshes();
        StringMap<Animation*> helper_get_all_animations();
        StringMap<Texture*> helper_get_all_textures();
        StringMap<Material*> helper_get_all_materials();
        StringMap<Shader*> helper_get_all_shaders();
        StringMap<Entity*> helper_get_all_entities();

        // Loading
        Optional<AssimpData> get_assimp_data(const StringView& path) const;
        void load_assimp_data(const AssimpData& data);

        Ref<Mesh> load_assimp_mesh(const aiScene* scene, const aiMesh* mesh);
		Ref<Animation> load_assimp_animation(const aiScene* scene, const aiAnimation* animation);
		Ref<Texture> load_assimp_texture(const aiScene* scene, const aiTexture* texture);
		Ref<Material> load_assimp_material(const aiScene* scene, const aiMaterial* material);

        template<typename T>
        static inline String generate_unique_name(const StringView& name, const StringMap<T>& map)
        {
            if (!map.contains(name))
                return String{ name };

            int i = 0;
            String result;
            do {
                i += 1;
                result = kl::format(name, '_', i);
            }
			while (map.contains(result));
			return result;
        }

    private:
        static px::PxDefaultAllocator m_allocator;
        static px::PxDefaultErrorCallback m_error_callback;
        static px::PxFoundation* m_foundation;

        px::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
        px::PxPhysics* m_physics = nullptr;
        px::PxCooking* m_cooking = nullptr;
        px::PxScene* m_scene = nullptr;

        kl::GPU* m_gpu = nullptr;
        StringMap<Ref<Entity>> m_entities;
    };
}
