#include "titian.h"


static px::PxFilterFlags filter_shader(
    px::PxFilterObjectAttributes attributes0,
    px::PxFilterData filterData0,
    px::PxFilterObjectAttributes attributes1,
    px::PxFilterData filterData1,
    px::PxPairFlags& pairFlags,
    void const* constantBlock,
    px::PxU32 constantBlockSize )
{
    pairFlags = px::PxPairFlag::eCONTACT_DEFAULT | px::PxPairFlag::eNOTIFY_TOUCH_FOUND;
    return px::PxFilterFlag::eDEFAULT;
}

static px::PxScene& make_px_scene( titian::Scene& parent )
{
    using namespace titian;
    px::PxPhysics& physics = AppLayer::get().physics;
    px::PxCpuDispatcher& cpu_dispatcher = AppLayer::get().cpu_dispatcher;
    px::PxSceneDesc scene_descriptor = { physics.getTolerancesScale() };
    scene_descriptor.gravity.y = -9.81f;
    scene_descriptor.cpuDispatcher = &cpu_dispatcher;
    scene_descriptor.filterShader = filter_shader;
    scene_descriptor.simulationEventCallback = &parent;
    return *physics.createScene( scene_descriptor );
}

titian::Scene::Scene()
    : m_scene( make_px_scene( *this ) )
{}

titian::Scene::~Scene()
{
    m_entities.clear();
    scripts.clear();
    materials.clear();
    textures.clear();
    meshes.clear();
    m_scene.release();
}

void titian::Scene::serialize( Serializer& serializer ) const
{
    serializer.write_string( "main_camera_name", main_camera_name );
    serializer.write_string( "main_ambient_light_name", main_ambient_light_name );
    serializer.write_string( "main_directional_light_name", main_directional_light_name );

    auto write_map = [&]<typename T>(StringRef const& map_name, StringMap<Ref<T>> const& data)
    {
        serializer.push_object( map_name );
        serializer.write_int( "data_size", (int32_t) data.size() );
        int counter = 0;
        for ( auto& [name, object] : data )
        {
            serializer.write_string( kl::format( "__name_", counter ), name );
            serializer.push_object( name );
            object->serialize( serializer );
            serializer.pop_object();
            counter += 1;
        }
        serializer.pop_object();
    };

    write_map( "meshes", meshes );
    write_map( "animations", animations );
    write_map( "textures", textures );
    write_map( "materials", materials );
    write_map( "shaders", shaders );
    write_map( "scripts", scripts );
    write_map( "entities", m_entities );
}

void titian::Scene::deserialize( Serializer const& serializer )
{
    serializer.read_string( "main_camera_name", main_camera_name );
    serializer.read_string( "main_ambient_light_name", main_ambient_light_name );
    serializer.read_string( "main_directional_light_name", main_directional_light_name );

    auto read_map = [&]<typename T>(StringRef const& map_name, StringMap<Ref<T>>&data, Func<T * ()> const& provider)
    {
        serializer.load_object( map_name );
        int32_t data_size = 0;
        serializer.read_int( "data_size", data_size );
        for ( int32_t i = 0; i < data_size; i++ )
        {
            String name;
            serializer.read_string( kl::format( "__name_", i ), name );
            Ref<T> object = provider();
            serializer.load_object( name );
            object->deserialize( serializer );
            serializer.unload_object();
            data[name] = object;
        }
        serializer.unload_object();
    };

    Func mesh_provider = [&] { return new Mesh(); };
    read_map( "meshes", meshes, mesh_provider );

    Func animation_provider = [&] { return new Animation(); };
    read_map( "animations", animations, animation_provider );

    Func texture_provider = [&] { return new Texture(); };
    read_map( "textures", textures, texture_provider );

    Func material_provider = [&] { return new Material(); };
    read_map( "materials", materials, material_provider );

    Func shader_provider = [&] { return new Shader(); };
    read_map( "shaders", shaders, shader_provider );

    {
        serializer.load_object( "scripts" );
        int32_t data_size = 0;
        serializer.read_int( "data_size", data_size );
        for ( int32_t i = 0; i < data_size; i++ )
        {
            String name;
            serializer.read_string( kl::format( "__name_", i ), name );
            serializer.load_object( name );
            String script_type;
            serializer.read_string( "script_type", script_type );
            Ref<Script> script;
            if ( typeid(InterpScript).name() == script_type )
            {
                script = new InterpScript();
            }
            else if ( typeid(NodeScript).name() == script_type )
            {
                script = new NodeScript();
            }
            else if ( typeid(NativeScript).name() == script_type )
            {
                script = new NativeScript();
            }
            else
            {
                kl::assert( false, "Unknown script type: ", script_type );
            }
            script->deserialize( serializer );
            serializer.unload_object();
            scripts[name] = script;
        }
        serializer.unload_object();
    }

    {
        serializer.load_object( "entities" );
        int32_t data_size = 0;
        serializer.read_int( "data_size", data_size );
        for ( int32_t i = 0; i < data_size; i++ )
        {
            String name;
            serializer.read_string( kl::format( "__name_", i ), name );
            serializer.load_object( name );
            String entity_type;
            serializer.read_string( "entity_type", entity_type );
            Ref<Entity> entity;
            if ( typeid(Entity).name() == entity_type )
            {
                entity = new Entity();
            }
            else if ( typeid(Camera).name() == entity_type )
            {
                entity = new Camera();
            }
            else if ( typeid(AmbientLight).name() == entity_type )
            {
                entity = new AmbientLight();
            }
            else if ( typeid(DirectionalLight).name() == entity_type )
            {
                entity = new DirectionalLight();
            }
            else
            {
                kl::assert( false, "Unknown entity type: ", entity_type );
            }
            entity->deserialize( serializer );
            serializer.unload_object();
            add_entity( name, entity );
        }
        serializer.unload_object();
    }
}

void titian::Scene::onConstraintBreak( px::PxConstraintInfo* constraints, px::PxU32 count )
{}

void titian::Scene::onWake( px::PxActor** actors, px::PxU32 count )
{}

void titian::Scene::onSleep( px::PxActor** actors, px::PxU32 count )
{}

void titian::Scene::onContact( px::PxContactPairHeader const& pairHeader, px::PxContactPair const* pairs, px::PxU32 nbPairs )
{
    Entity* entity_0 = (Entity*) pairHeader.actors[0]->userData;
    Entity* entity_1 = (Entity*) pairHeader.actors[1]->userData;
    if ( !entity_0 || !entity_1 )
        return;

    for ( auto& script : scripts | std::views::values )
        script->call_collision( *this, *entity_0, *entity_1 );
}

void titian::Scene::onTrigger( px::PxTriggerPair* pairs, px::PxU32 count )
{}

void titian::Scene::onAdvance( px::PxRigidBody const* const* bodyBuffer, px::PxTransform const* poseBuffer, px::PxU32 count )
{}

void titian::Scene::set_gravity( Float3 const& gravity )
{
    m_scene.setGravity( px_cast( gravity ) );
}

titian::Float3 titian::Scene::gravity() const
{
    return px_cast( m_scene.getGravity() );
}

void titian::Scene::update_physics( float delta_t )
{
    m_scene.simulate( delta_t );
    m_scene.fetchResults( true );
}

void titian::Scene::update_scripts()
{
    for ( auto& script : scripts | std::views::values )
        script->call_update( *this );
}

void titian::Scene::update_ui()
{
    for ( auto& script : scripts | std::views::values )
        script->call_ui( *this );
}

void titian::Scene::add_entity( String const& id, Ref<Entity> const& entity )
{
    remove_entity( id );
    if ( !entity )
        return;

    m_entities[id] = entity;
    m_scene.addActor( entity->actor() );
    entity->wake_up();
}

void titian::Scene::remove_entity( StringRef const& id )
{
    auto it = m_entities.find( id );
    if ( it == m_entities.end() )
        return;

    m_scene.removeActor( it->second->actor() );
    m_entities.erase( it );
}

titian::Mesh& titian::Scene::helper_new_mesh( String const& id )
{
    Mesh* mesh = new Mesh();
    meshes[id] = mesh;
    return *mesh;
}

titian::Animation& titian::Scene::helper_new_animation( String const& id )
{
    Animation* animation = new Animation();
    animations[id] = animation;
    return *animation;
}

titian::Texture& titian::Scene::helper_new_texture( String const& id )
{
    Texture* texture = new Texture();
    textures[id] = texture;
    return *texture;
}

titian::Material& titian::Scene::helper_new_material( String const& id )
{
    Material* material = new Material();
    materials[id] = material;
    return *material;
}

titian::Shader& titian::Scene::helper_new_shader( String const& id )
{
    Shader* shader = new Shader();
    shaders[id] = shader;
    return *shader;
}

titian::Entity& titian::Scene::helper_new_entity( String const& id )
{
    Entity* entity = new Entity();
    add_entity( id, entity );
    return *entity;
}

titian::Mesh* titian::Scene::helper_get_mesh( StringRef const& id ) const
{
    auto it = meshes.find( id );
    if ( it != meshes.end() )
    {
        return &it->second;
    }
    return nullptr;
}

titian::Animation* titian::Scene::helper_get_animation( StringRef const& id ) const
{
    auto it = animations.find( id );
    if ( it != animations.end() )
    {
        return &it->second;
    }
    return nullptr;
}

titian::Texture* titian::Scene::helper_get_texture( StringRef const& id ) const
{
    auto it = textures.find( id );
    if ( it != textures.end() )
    {
        return &it->second;
    }
    return nullptr;
}

titian::Material* titian::Scene::helper_get_material( StringRef const& id ) const
{
    auto it = materials.find( id );
    if ( it != materials.end() )
    {
        return &it->second;
    }
    return nullptr;
}

titian::Shader* titian::Scene::helper_get_shader( StringRef const& id ) const
{
    auto it = shaders.find( id );
    if ( it != shaders.end() )
    {
        return &it->second;
    }
    return nullptr;
}

titian::Entity* titian::Scene::helper_get_entity( StringRef const& id ) const
{
    auto it = m_entities.find( id );
    if ( it != m_entities.end() )
    {
        return &it->second;
    }
    return nullptr;
}

void titian::Scene::helper_remove_mesh( StringRef const& id )
{
    auto it = meshes.find( id );
    if ( it != meshes.end() )
    {
        meshes.erase( it );
    }
}

void titian::Scene::helper_remove_animation( StringRef const& id )
{
    auto it = animations.find( id );
    if ( it != animations.end() )
    {
        animations.erase( it );
    }
}

void titian::Scene::helper_remove_texture( StringRef const& id )
{
    auto it = textures.find( id );
    if ( it != textures.end() )
    {
        textures.erase( it );
    }
}

void titian::Scene::helper_remove_material( StringRef const& id )
{
    auto it = materials.find( id );
    if ( it != materials.end() )
    {
        materials.erase( it );
    }
}

void titian::Scene::helper_remove_shader( StringRef const& id )
{
    auto it = shaders.find( id );
    if ( it != shaders.end() )
    {
        shaders.erase( it );
    }
}

void titian::Scene::helper_remove_entity( StringRef const& id )
{
    remove_entity( id );
}

bool titian::Scene::helper_contains_mesh( StringRef const& id ) const
{
    return meshes.contains( id );
}

bool titian::Scene::helper_contains_animation( StringRef const& id ) const
{
    return animations.contains( id );
}

bool titian::Scene::helper_contains_texture( StringRef const& id ) const
{
    return textures.contains( id );
}

bool titian::Scene::helper_contains_material( StringRef const& id ) const
{
    return materials.contains( id );
}

bool titian::Scene::helper_contains_shader( StringRef const& id ) const
{
    return shaders.contains( id );
}

bool titian::Scene::helper_contains_entity( StringRef const& id ) const
{
    return m_entities.contains( id );
}

void titian::Scene::helper_iterate_meshes( Func<void( String const&, Mesh* )> const& func )
{
    std::ranges::for_each( meshes, [&]( auto& entry ) { func( entry.first, &entry.second ); } );
}

void titian::Scene::helper_iterate_animations( Func<void( String const&, Animation* )> const& func )
{
    std::ranges::for_each( animations, [&]( auto& entry ) { func( entry.first, &entry.second ); } );
}

void titian::Scene::helper_iterate_textures( Func<void( String const&, Texture* )> const& func )
{
    std::ranges::for_each( textures, [&]( auto& entry ) { func( entry.first, &entry.second ); } );
}

void titian::Scene::helper_iterate_materials( Func<void( String const&, Material* )> const& func )
{
    std::ranges::for_each( materials, [&]( auto& entry ) { func( entry.first, &entry.second ); } );
}

void titian::Scene::helper_iterate_shaders( Func<void( String const&, Shader* )> const& func )
{
    std::ranges::for_each( shaders, [&]( auto& entry ) { func( entry.first, &entry.second ); } );
}

void titian::Scene::helper_iterate_entities( Func<void( String const&, Entity* )> const& func )
{
    std::ranges::for_each( m_entities, [&]( auto& entry ) { func( entry.first, &entry.second ); } );
}

titian::Opt<titian::AssimpData> titian::Scene::get_assimp_data( StringRef const& path ) const
{
    Ref importer = new as::Importer();
    aiScene const* scene = importer->ReadFile( path,
        aiProcess_ConvertToLeftHanded |
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_GenNormals );
    if ( !scene )
        return std::nullopt;

    AssimpData data{};
    data.importer = importer;
    for ( uint32_t i = 0; i < scene->mNumMeshes; i++ )
    {
        aiMesh const& mesh = *scene->mMeshes[i];
        String name = mesh.mName.C_Str();
        if ( name.empty() )
        {
            name = "unknown_mesh";
        }
        data.meshes.push_back( generate_unique_name( name, meshes ) );
    }
    for ( uint32_t i = 0; i < scene->mNumAnimations; i++ )
    {
        aiAnimation* animation = scene->mAnimations[i];
        String name = animation->mName.C_Str();
        if ( name.empty() )
        {
            name = "unknown_animation";
        }
        data.animations.push_back( generate_unique_name( name, animations ) );
    }
    for ( uint32_t i = 0; i < scene->mNumTextures; i++ )
    {
        aiTexture* texture = scene->mTextures[i];
        String name = texture->mFilename.C_Str();
        if ( name.empty() )
        {
            name = "unknown_texture";
        }
        data.textures.push_back( generate_unique_name( name, textures ) );
    }
    for ( uint32_t i = 0; i < scene->mNumMaterials; i++ )
    {
        aiMaterial* material = scene->mMaterials[i];
        String name = material->GetName().C_Str();
        if ( name.empty() )
        {
            name = "unknown_material";
        }
        data.materials.push_back( generate_unique_name( name, materials ) );
    }
    return data;
}

void titian::Scene::load_assimp_data( AssimpData const& data )
{
    aiScene const* scene = data.importer->GetScene();
    for ( uint32_t i = 0; i < scene->mNumMeshes; i++ )
        meshes[data.meshes[i]] = load_assimp_mesh( *scene, *scene->mMeshes[i] );
    for ( uint32_t i = 0; i < scene->mNumAnimations; i++ )
        animations[data.animations[i]] = load_assimp_animation( *scene, *scene->mAnimations[i] );
    for ( uint32_t i = 0; i < scene->mNumTextures; i++ )
        textures[data.textures[i]] = load_assimp_texture( *scene, *scene->mTextures[i] );
    for ( uint32_t i = 0; i < scene->mNumMaterials; i++ )
        materials[data.materials[i]] = load_assimp_material( *scene, *scene->mMaterials[i] );
}

titian::Ref<titian::Mesh> titian::Scene::load_assimp_mesh( aiScene const& scene, aiMesh const& mesh )
{
    Ref mesh_object = new Mesh();

    Vector<Vertex> vertices( mesh.mNumVertices );
    if ( mesh.HasPositions() )
    {
        for ( uint32_t i = 0; i < mesh.mNumVertices; i++ )
            vertices[i].position = { mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z };
    }
    if ( mesh.HasNormals() )
    {
        for ( uint32_t i = 0; i < mesh.mNumVertices; i++ )
            vertices[i].normal = { mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z };
    }
    if ( mesh.HasTextureCoords( 0 ) )
    {
        for ( uint32_t i = 0; i < mesh.mNumVertices; i++ )
            vertices[i].uv = { mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y };
    }
    if ( mesh.HasBones() )
    {
        if ( mesh.mNumBones > MAX_BONE_COUNT )
            Logger::log( "Mesh has too many bones: ", mesh.mNumBones, " > ", MAX_BONE_COUNT );

        for ( uint32_t i = 0; i < mesh.mNumBones; i++ )
        {
            auto& bone = mesh.mBones[i];
            for ( uint32_t j = 0; j < bone->mNumWeights; j++ )
            {
                auto& weight = bone->mWeights[j];
                auto& vertex = vertices[weight.mVertexId];
                for ( int k = 0; k < MAX_BONE_REFS; k++ )
                {
                    if ( vertex.bone_weights[k] == 0.0f )
                    {
                        vertex.bone_weights[k] = (float) weight.mWeight;
                        vertex.bone_indices[k] = (byte) i;
                        break;
                    }
                }
            }
        }

        mesh_object->bone_matrices.resize( mesh.mNumBones );
        for ( uint32_t i = 0; i < mesh.mNumBones; i++ )
        {
            auto& bone = mesh.mBones[i];
            auto& offset_matrix = mesh_object->bone_matrices[i];
            for ( int j = 0; j < 4; j++ )
                kl::copy<float>( &offset_matrix( 0, j ), bone->mOffsetMatrix[j], 4 );
        }

        Func<Ref<SkeletonNode>( aiNode* )> recur_helper;
        recur_helper = [&]( aiNode* node )
        {
            Ref<SkeletonNode> skeleton_node = new SkeletonNode();
            skeleton_node->bone_index = -1;
            for ( uint32_t i = 0; i < mesh.mNumBones; i++ )
            {
                if ( mesh.mBones[i]->mName == node->mName )
                {
                    skeleton_node->bone_index = i;
                    break;
                }
            }
            for ( int i = 0; i < 4; i++ )
                kl::copy<float>( &skeleton_node->transformation( 0, i ), node->mTransformation[i], 4 );

            skeleton_node->children.resize( node->mNumChildren );
            for ( uint32_t i = 0; i < node->mNumChildren; i++ )
                skeleton_node->children[i] = recur_helper( node->mChildren[i] );

            return skeleton_node;
        };
        mesh_object->skeleton_root = recur_helper( scene.mRootNode );
    }

    mesh_object->vertices.reserve( (size_t) mesh.mNumFaces * 3 );
    for ( uint32_t i = 0; i < mesh.mNumFaces; i++ )
    {
        auto& face = mesh.mFaces[i];
        for ( uint32_t j = 0; j < face.mNumIndices; j++ )
        {
            uint32_t index = face.mIndices[j];
            mesh_object->vertices.push_back( vertices[index] );
        }
    }

    mesh_object->reload();
    return mesh_object;
}

titian::Ref<titian::Animation> titian::Scene::load_assimp_animation( aiScene const& scene, aiAnimation const& animation )
{
    Ref animation_object = new Animation();

    animation_object->ticks_per_second = (float) animation.mTicksPerSecond;
    animation_object->duration_in_ticks = (float) animation.mDuration;

    animation_object->channels.resize( animation.mNumChannels );
    for ( uint32_t i = 0; i < animation.mNumChannels; i++ )
    {
        auto& channel = animation.mChannels[i];
        auto& [scalings, rotations, positions] = animation_object->channels[i];

        scalings.resize( channel->mNumScalingKeys );
        for ( uint32_t j = 0; j < channel->mNumScalingKeys; j++ )
        {
            auto& key = channel->mScalingKeys[j];
            scalings[j].first = (float) key.mTime;
            scalings[j].second = key.mValue;
        }

        rotations.resize( channel->mNumRotationKeys );
        for ( uint32_t j = 0; j < channel->mNumRotationKeys; j++ )
        {
            auto& key = channel->mRotationKeys[j];
            rotations[j].first = (float) key.mTime;
            rotations[j].second = key.mValue;
        }

        positions.resize( channel->mNumPositionKeys );
        for ( uint32_t j = 0; j < channel->mNumPositionKeys; j++ )
        {
            auto& key = channel->mPositionKeys[j];
            positions[j].first = (float) key.mTime;
            positions[j].second = key.mValue;
        }
    }

    Func<Ref<AnimationNode>( aiNode* )> recur_helper;
    recur_helper = [&]( aiNode* node )
    {
        Ref<AnimationNode> animation_node = new AnimationNode();
        animation_node->channel_index = -1;
        for ( uint32_t i = 0; i < animation.mNumChannels; i++ )
        {
            if ( animation.mChannels[i]->mNodeName == node->mName )
            {
                animation_node->channel_index = i;
                break;
            }
        }

        animation_node->children.resize( node->mNumChildren );
        for ( uint32_t i = 0; i < node->mNumChildren; i++ )
            animation_node->children[i] = recur_helper( node->mChildren[i] );

        return animation_node;
    };
    animation_object->animation_root = recur_helper( scene.mRootNode );

    return animation_object;
}

titian::Ref<titian::Texture> titian::Scene::load_assimp_texture( aiScene const& scene, aiTexture const& texture )
{
    Ref texture_object = new Texture();
    if ( texture.mHeight == 0 )
    {
        texture_object->image.load_from_memory( texture.pcData, texture.mWidth );
    }
    else
    {
        texture_object->image.resize( { (int) texture.mWidth, (int) texture.mHeight } );
        kl::copy<RGB>( texture_object->image.ptr(), texture.pcData, texture_object->image.pixel_count() );
    }
    texture_object->reload_as_2D();
    texture_object->create_shader_view();
    return texture_object;
}

titian::Ref<titian::Material> titian::Scene::load_assimp_material( aiScene const& scene, aiMaterial const& material )
{
    Ref material_object = new Material();
    material.Get( AI_MATKEY_COLOR_DIFFUSE, material_object->color );
    material.Get( AI_MATKEY_COLOR_TRANSPARENT, material_object->color.w );
    material.Get( AI_MATKEY_REFLECTIVITY, material_object->reflectivity_factor );
    material.Get( AI_MATKEY_REFRACTI, material_object->refraction_index );
    return material_object;
}
