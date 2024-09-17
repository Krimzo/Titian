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
    struct Scene : kl::NoCopy, Serializable, px::PxSimulationEventCallback
    {
        StringMap<Ref<Mesh>> meshes;
        StringMap<Ref<Animation>> animations;
        StringMap<Ref<Texture>> textures;
        StringMap<Ref<Material>> materials;
        StringMap<Ref<Shader>> shaders;
        StringMap<Ref<Script>> scripts;

        Ref<DefaultMeshes> default_meshes;
        Ref<DefaultAnimations> default_animations;
        Ref<DefaultMaterials> default_materials;

        String main_camera_name = "/";
        String main_ambient_light_name = "/";
        String main_directional_light_name = "/";

        Scene();
        ~Scene() override;

        void serialize(Serializer* serializer) const override;
        void deserialize(const Serializer* serializer) override;
        
        void onConstraintBreak(px::PxConstraintInfo* constraints, px::PxU32 count) override;
        void onWake(px::PxActor** actors, px::PxU32 count) override;
        void onSleep(px::PxActor** actors, px::PxU32 count) override;
        void onContact(const px::PxContactPairHeader& pairHeader, const px::PxContactPair* pairs, px::PxU32 nbPairs) override;
        void onTrigger(px::PxTriggerPair* pairs, px::PxU32 count) override;
        void onAdvance(const px::PxRigidBody* const* bodyBuffer, const px::PxTransform* poseBuffer, const px::PxU32 count) override;

        void set_gravity(const Float3& gravity);
        Float3 gravity() const;

        void update_physics(float delta_t);
        void update_scripts();
        void update_ui();

        void add_entity(const String& id, const Ref<Entity>& entity);
        void remove_entity(const StringView& id);
        inline const auto& entities() const { return m_entities; };

        template<typename T>
        T* get_casted(const StringView& id)
        {
            return dynamic_cast<T*>(helper_get_entity(id));
        }

        Ref<Collider> new_box_collider(const Float3& scale) const;
        Ref<Collider> new_sphere_collider(float radius) const;
        Ref<Collider> new_capsule_collider(float radius, float height) const;
        Ref<Collider> new_collider(px::PxGeometryType::Enum type) const;

        Mesh* helper_new_mesh(const String& id);
        Animation* helper_new_animation(const String& id);
        Texture* helper_new_texture(const String& id);
        Material* helper_new_material(const String& id);
        Shader* helper_new_shader(const String& id);
        Entity* helper_new_entity(const String& id);

        Mesh* helper_get_mesh(const StringView& id);
        Animation* helper_get_animation(const StringView& id);
        Texture* helper_get_texture(const StringView& id);
        Material* helper_get_material(const StringView& id);
        Shader* helper_get_shader(const StringView& id);
        Entity* helper_get_entity(const StringView& id);

        void helper_remove_mesh(const StringView& id);
        void helper_remove_animation(const StringView& id);
        void helper_remove_texture(const StringView& id);
        void helper_remove_material(const StringView& id);
        void helper_remove_shader(const StringView& id);
        void helper_remove_entity(const StringView& id);

        bool helper_contains_mesh(const StringView& id) const;
        bool helper_contains_animation(const StringView& id) const;
        bool helper_contains_texture(const StringView& id) const;
        bool helper_contains_material(const StringView& id) const;
        bool helper_contains_shader(const StringView& id) const;
        bool helper_contains_entity(const StringView& id) const;

        void helper_iterate_meshes(const Function<void(const String&, Mesh*)>& func);
        void helper_iterate_animations(const Function<void(const String&, Animation*)>& func);
        void helper_iterate_textures(const Function<void(const String&, Texture*)>& func);
        void helper_iterate_materials(const Function<void(const String&, Material*)>& func);
        void helper_iterate_shaders(const Function<void(const String&, Shader*)>& func);
        void helper_iterate_entities(const Function<void(const String&, Entity*)>& func);

        Optional<AssimpData> get_assimp_data(const StringView& path) const;
        void load_assimp_data(const AssimpData& data);

        Ref<Mesh> load_assimp_mesh(const aiScene* scene, const aiMesh* mesh);
		Ref<Animation> load_assimp_animation(const aiScene* scene, const aiAnimation* animation);
		Ref<Texture> load_assimp_texture(const aiScene* scene, const aiTexture* texture);
		Ref<Material> load_assimp_material(const aiScene* scene, const aiMaterial* material);

        template<typename T>
        static String generate_unique_name(const StringView& name, const StringMap<T>& map)
        {
            if (!map.contains(name))
                return String{ name };

            int i = 0;
            String result;
            do {
                i += 1;
                result = kl::format(name, i);
            }
			while (map.contains(result));
			return result;
        }

    private:
        px::PxScene* m_scene = nullptr;
        StringMap<Ref<Entity>> m_entities;
    };
}
