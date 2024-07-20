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
        Ref<as::Importer> importer;
        Vector<String> meshes;
        Vector<String> animations;
        Vector<String> textures;
        Vector<String> materials;
    };
}

namespace titian {
    class Scene : public Serializable, public px::PxSimulationEventCallback
    {
    public:
        Map<String, Ref<Mesh>> meshes;
        Map<String, Ref<Animation>> animations;
        Map<String, Ref<Texture>> textures;
        Map<String, Ref<Material>> materials;
        Map<String, Ref<Shader>> shaders;
        Map<String, Ref<Script>> scripts;

        Ref<DefaultMeshes> default_meshes = nullptr;
        Ref<DefaultAnimations> default_animations = nullptr;
        Ref<DefaultMaterials> default_materials = nullptr;

        String main_camera_name = "/";
        String main_ambient_light_name = "/";
        String main_directional_light_name = "/";

        Scene(kl::GPU* gpu);
        ~Scene() override;

        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;

        void operator=(const Scene&) = delete;
        void operator=(Scene&&) = delete;

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
        void set_gravity(const kl::Float3& gravity);
        kl::Float3 gravity() const;

        // Update
        void update_physics(float delta_t);
        void update_scripts();
        void update_ui();

        // Entity
        Ref<Entity> new_entity(bool dynamic) const;
        void add_entity(const String& name, const Ref<Entity>& entity);
        void remove_entity(const String& name);
        bool contains_entity(const String& name) const;
        size_t entity_count() const;

        Map<String, Ref<Entity>>::iterator begin();
        Map<String, Ref<Entity>>::iterator end();
        Map<String, Ref<Entity>>::const_iterator begin() const;
        Map<String, Ref<Entity>>::const_iterator end() const;

        // Get types
        Ref<Mesh> get_mesh(const String& id) const;
        Ref<Animation> get_animation(const String& id) const;
        Ref<Texture> get_texture(const String& id) const;
        Ref<Material> get_material(const String& id) const;
        Ref<Shader> get_shader(const String& id) const;
        Ref<Script> get_script(const String& id) const;
        Ref<Entity> get_entity(const String& id) const;

        // Casted
        template<typename T, typename... Args>
        Ref<T> new_casted(const bool dynamic, const Args&... args) const
        {
            return new T(m_physics, dynamic, args...);
        }

        template<typename T>
        T* get_casted(const String& id)
        {
            Entity* entity = &get_entity(id);
            return dynamic_cast<T*>(entity);
        }

        template<typename T>
        const T* get_casted(const String& id) const
        {
            const Entity* entity = &get_entity(id);
            return dynamic_cast<const T*>(entity);
        }

        // Dynamic colliders
        Ref<Collider> new_box_collider(const kl::Float3& scale) const;
        Ref<Collider> new_sphere_collider(float radius) const;
        Ref<Collider> new_capsule_collider(float radius, float height) const;

        // Static colliders
        Ref<Collider> new_mesh_collider(const Mesh& mesh, const kl::Float3& scale) const;

        // Default collider
        Ref<Collider> new_default_collider(px::PxGeometryType::Enum type, const Mesh* optional_mesh) const;

        // Helper new
        Mesh* helper_new_mesh(const String& id);
        Animation* helper_new_animation(const String& id);
        Texture* helper_new_texture(const String& id);
        Material* helper_new_material(const String& id);
        Shader* helper_new_shader(const String& id);
        Entity* helper_new_entity(const String& id);

        // Helper get
        Mesh* helper_get_mesh(const String& id);
        Animation* helper_get_animation(const String& id);
        Texture* helper_get_texture(const String& id);
        Material* helper_get_material(const String& id);
        Shader* helper_get_shader(const String& id);
        Entity* helper_get_entity(const String& id);

        // Helper remove
        void helper_remove_mesh(const String& id);
        void helper_remove_animation(const String& id);
        void helper_remove_texture(const String& id);
        void helper_remove_material(const String& id);
        void helper_remove_shader(const String& id);
        void helper_remove_entity(const String& id);

        // Helper contains
        bool helper_contains_mesh(const String& id) const;
        bool helper_contains_animation(const String& id) const;
        bool helper_contains_texture(const String& id) const;
        bool helper_contains_material(const String& id) const;
        bool helper_contains_shader(const String& id) const;
        bool helper_contains_entity(const String& id) const;

        // Helper count
        int helper_mesh_count() const;
        int helper_animation_count() const;
        int helper_texture_count() const;
        int helper_material_count() const;
        int helper_shader_count() const;
        int helper_entity_count() const;

        // Helper get all
        Map<String, Mesh*> helper_get_all_meshes();
        Map<String, Animation*> helper_get_all_animations();
        Map<String, Texture*> helper_get_all_textures();
        Map<String, Material*> helper_get_all_materials();
        Map<String, Shader*> helper_get_all_shaders();
        Map<String, Entity*> helper_get_all_entities();

        // Loading
        Optional<AssimpData> get_assimp_data(const String& path) const;
        void load_assimp_data(const AssimpData& data);

        Ref<Mesh> load_assimp_mesh(const aiScene* scene, const aiMesh* mesh);
		Ref<Animation> load_assimp_animation(const aiScene* scene, const aiAnimation* animation);
		Ref<Texture> load_assimp_texture(const aiScene* scene, const aiTexture* texture);
		Ref<Material> load_assimp_material(const aiScene* scene, const aiMaterial* material);

        template<typename T>
        static inline String generate_unique_name(const String& name, const Map<String, T>& map)
        {
            if (!map.contains(name))
                return name;

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
        Map<String, Ref<Entity>> m_entities = {};
    };
}
