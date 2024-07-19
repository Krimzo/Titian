#pragma once

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
        kl::Object<Assimp::Importer> importer;
        std::vector<std::string> meshes;
        std::vector<std::string> animations;
        std::vector<std::string> textures;
        std::vector<std::string> materials;
    };
}

namespace titian {
    class Scene : public Serializable, public physx::PxSimulationEventCallback
    {
    public:
        std::map<std::string, kl::Object<Mesh>> meshes = {};
        std::map<std::string, kl::Object<Animation>> animations = {};
        std::map<std::string, kl::Object<Texture>> textures = {};
        std::map<std::string, kl::Object<Material>> materials = {};
        std::map<std::string, kl::Object<Shader>> shaders = {};
        std::map<std::string, kl::Object<Script>> scripts = {};

        kl::Object<DefaultMeshes> default_meshes = nullptr;
        kl::Object<DefaultAnimations> default_animations = nullptr;
        kl::Object<DefaultMaterials> default_materials = nullptr;

        std::string main_camera_name = "/";
        std::string main_ambient_light_name = "/";
        std::string main_directional_light_name = "/";

        Scene(kl::GPU* gpu);
        ~Scene() override;

        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;

        void operator=(const Scene&) = delete;
        void operator=(Scene&&) = delete;

        // Overrides
        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;
        
        void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
        void onWake(physx::PxActor** actors, physx::PxU32 count) override;
        void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
        void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
        void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
        void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

        // Get ptrs
        physx::PxPhysics* physics() const;
        physx::PxCooking* cooking() const;

        // Gravity
        void set_gravity(const kl::Float3& gravity);
        kl::Float3 gravity() const;

        // Update
        void update_physics(float delta_t);
        void update_scripts();
        void update_ui();

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
        kl::Object<Animation> get_animation(const std::string& id) const;
        kl::Object<Texture> get_texture(const std::string& id) const;
        kl::Object<Material> get_material(const std::string& id) const;
        kl::Object<Shader> get_shader(const std::string& id) const;
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
        Animation* helper_new_animation(const std::string& id);
        Texture* helper_new_texture(const std::string& id);
        Material* helper_new_material(const std::string& id);
        Shader* helper_new_shader(const std::string& id);
        Entity* helper_new_entity(const std::string& id);

        // Helper get
        Mesh* helper_get_mesh(const std::string& id);
        Animation* helper_get_animation(const std::string& id);
        Texture* helper_get_texture(const std::string& id);
        Material* helper_get_material(const std::string& id);
        Shader* helper_get_shader(const std::string& id);
        Entity* helper_get_entity(const std::string& id);

        // Helper remove
        void helper_remove_mesh(const std::string& id);
        void helper_remove_animation(const std::string& id);
        void helper_remove_texture(const std::string& id);
        void helper_remove_material(const std::string& id);
        void helper_remove_shader(const std::string& id);
        void helper_remove_entity(const std::string& id);

        // Helper contains
        bool helper_contains_mesh(const std::string& id) const;
        bool helper_contains_animation(const std::string& id) const;
        bool helper_contains_texture(const std::string& id) const;
        bool helper_contains_material(const std::string& id) const;
        bool helper_contains_shader(const std::string& id) const;
        bool helper_contains_entity(const std::string& id) const;

        // Helper count
        int helper_mesh_count() const;
        int helper_animation_count() const;
        int helper_texture_count() const;
        int helper_material_count() const;
        int helper_shader_count() const;
        int helper_entity_count() const;

        // Helper get all
        std::map<std::string, Mesh*> helper_get_all_meshes();
        std::map<std::string, Animation*> helper_get_all_animations();
        std::map<std::string, Texture*> helper_get_all_textures();
        std::map<std::string, Material*> helper_get_all_materials();
        std::map<std::string, Shader*> helper_get_all_shaders();
        std::map<std::string, Entity*> helper_get_all_entities();

        // Loading
        std::optional<AssimpData> get_assimp_data(const std::string& path) const;
        void load_assimp_data(const AssimpData& data);

        kl::Object<Mesh> load_assimp_mesh(const aiScene* scene, const aiMesh* mesh);
		kl::Object<Animation> load_assimp_animation(const aiScene* scene, const aiAnimation* animation);
		kl::Object<Texture> load_assimp_texture(const aiScene* scene, const aiTexture* texture);
		kl::Object<Material> load_assimp_material(const aiScene* scene, const aiMaterial* material);

        template<typename T>
        static inline std::string generate_unique_name(const std::string& name, const std::map<std::string, T>& map)
        {
            if (!map.contains(name))
                return name;

            int i = 0;
            std::string result;
            do {
                i += 1;
                result = kl::format(name, '_', i);
            }
			while (map.contains(result));
			return result;
        }

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
