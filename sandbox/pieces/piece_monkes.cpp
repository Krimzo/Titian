#include "sandbox.h"


titian::SandboxPieceMonkes::SandboxPieceMonkes( TitianEditor& editor, int size )
    : SandboxPiece( editor )
{
    this->size = size;
}

void titian::SandboxPieceMonkes::setup_self()
{
    kl::GPU& gpu = editor.app_layer.gpu;
    Scene& scene = editor.game_layer.scene();

    int entity_count = size * size;
    int half_size = size / 2;

    Ref<Mesh> monke_mesh;
    if ( auto opt_data = scene.get_assimp_data( "package/meshes/monke.obj" ) )
    {
        aiScene const& ai_scene = *opt_data.value().importer->GetScene();
        for ( uint32_t i = 0; i < ai_scene.mNumMeshes; i++ )
        {
            monke_mesh = scene.load_assimp_mesh( ai_scene, *ai_scene.mMeshes[i] );
            break;
        }
    }
    if ( !monke_mesh )
        return;

    Ref monke_animation = new Animation();
    monke_animation->meshes = { "monke" };

    scene.meshes["monke"] = monke_mesh;
    scene.animations["monke"] = monke_animation;

    for ( int i = 0; i < entity_count; i++ )
    {
        int x = i % size;
        int y = i / size;

        String mesh_name = "monke";
        String animation_name = "monke";
        String counter_id = kl::format( std::setw( 3 ), std::setfill( '0' ), i );
        String material_name = kl::format( "monke_mat_", counter_id );
        String entity_name = kl::format( "Monke", counter_id );

        Ref material = new Material();
        material->color = kl::colors::CYAN;
        material->reflectivity_factor = -i / (entity_count - 1.0f);
        material->refraction_index = 1.5f;
        scene.materials[material_name] = material;

        Ref monke = new Entity();
        monke->set_scale( Float3{ 0.5f } );
        monke->set_position( {
            (float) (x - half_size) + x_offset,
            (float) (y - half_size),
            5.0f,
            } );

        monke->animation_name = animation_name;
        monke->material_name = material_name;
        scene.add_entity( entity_name, monke );
    }
}
