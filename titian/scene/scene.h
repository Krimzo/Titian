#pragma once

#include "scene/components/mesh.h"
#include "scene/components/animation.h"
#include "scene/components/texture.h"
#include "scene/components/material.h"
#include "scene/components/shader.h"
#include "script/script.h"
#include "scene/scene_defaults.h"
#include "scene/entity.h"


namespace titian
{
struct AssimpData
{
    Ref<as::Importer> importer;
    Vector<String> meshes;
    Vector<String> animations;
    Vector<String> textures;
    Vector<String> materials;
};
}

namespace titian
{
struct Scene : kl::NoCopy, Serializable, px::PxSimulationEventCallback
{
    StringMap<Ref<Mesh>> meshes;
    StringMap<Ref<Animation>> animations;
    StringMap<Ref<Texture>> textures;
    StringMap<Ref<Material>> materials;
    StringMap<Ref<Shader>> shaders;
    StringMap<Ref<Script>> scripts;

    DefaultMeshes default_meshes;
    DefaultAnimations default_animations;
    DefaultMaterials default_materials;

    String main_camera_name = "/";
    String main_ambient_light_name = "/";
    String main_directional_light_name = "/";

    Scene();
    ~Scene() override;

    void serialize( Serializer& serializer ) const override;
    void deserialize( Serializer const& serializer ) override;

    void onConstraintBreak( px::PxConstraintInfo* constraints, px::PxU32 count ) override;
    void onWake( px::PxActor** actors, px::PxU32 count ) override;
    void onSleep( px::PxActor** actors, px::PxU32 count ) override;
    void onContact( px::PxContactPairHeader const& pairHeader, px::PxContactPair const* pairs, px::PxU32 nbPairs ) override;
    void onTrigger( px::PxTriggerPair* pairs, px::PxU32 count ) override;
    void onAdvance( px::PxRigidBody const* const* bodyBuffer, px::PxTransform const* poseBuffer, px::PxU32 count ) override;

    void set_gravity( Float3 const& gravity );
    Float3 gravity() const;

    void update_physics( float delta_t );
    void update_scripts();
    void update_ui();

    void add_entity( String const& id, Ref<Entity> const& entity );
    void remove_entity( StringRef const& id );
    inline auto& entities() const { return m_entities; };

    template<typename T>
    T* get_casted( StringRef const& id )
    {
        return dynamic_cast<T*>(helper_get_entity( id ));
    }

    Mesh& helper_new_mesh( String const& id );
    Animation& helper_new_animation( String const& id );
    Texture& helper_new_texture( String const& id );
    Material& helper_new_material( String const& id );
    Shader& helper_new_shader( String const& id );
    Entity& helper_new_entity( String const& id );

    Mesh* helper_get_mesh( StringRef const& id ) const;
    Animation* helper_get_animation( StringRef const& id ) const;
    Texture* helper_get_texture( StringRef const& id ) const;
    Material* helper_get_material( StringRef const& id ) const;
    Shader* helper_get_shader( StringRef const& id ) const;
    Entity* helper_get_entity( StringRef const& id ) const;

    void helper_remove_mesh( StringRef const& id );
    void helper_remove_animation( StringRef const& id );
    void helper_remove_texture( StringRef const& id );
    void helper_remove_material( StringRef const& id );
    void helper_remove_shader( StringRef const& id );
    void helper_remove_entity( StringRef const& id );

    bool helper_contains_mesh( StringRef const& id ) const;
    bool helper_contains_animation( StringRef const& id ) const;
    bool helper_contains_texture( StringRef const& id ) const;
    bool helper_contains_material( StringRef const& id ) const;
    bool helper_contains_shader( StringRef const& id ) const;
    bool helper_contains_entity( StringRef const& id ) const;

    void helper_iterate_meshes( Func<void( String const&, Mesh* )> const& func );
    void helper_iterate_animations( Func<void( String const&, Animation* )> const& func );
    void helper_iterate_textures( Func<void( String const&, Texture* )> const& func );
    void helper_iterate_materials( Func<void( String const&, Material* )> const& func );
    void helper_iterate_shaders( Func<void( String const&, Shader* )> const& func );
    void helper_iterate_entities( Func<void( String const&, Entity* )> const& func );

    Opt<AssimpData> get_assimp_data( StringRef const& path ) const;
    void load_assimp_data( AssimpData const& data );

    Ref<Mesh> load_assimp_mesh( aiScene const& scene, aiMesh const& mesh );
    Ref<Animation> load_assimp_animation( aiScene const& scene, aiAnimation const& animation );
    Ref<Texture> load_assimp_texture( aiScene const& scene, aiTexture const& texture );
    Ref<Material> load_assimp_material( aiScene const& scene, aiMaterial const& material );

    template<typename T>
    static String generate_unique_name( StringRef const& name, StringMap<T> const& map )
    {
        if ( !map.contains( name ) )
            return String{ name };

        int i = 0;
        String result;
        do
        {
            i += 1;
            result = kl::format( name, i );
        }
        while ( map.contains( result ) );
        return result;
    }

private:
    px::PxScene& m_scene;
    StringMap<Ref<Entity>> m_entities;
};
}
