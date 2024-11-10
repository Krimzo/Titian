#pragma once

#include "scene/scene.h"


namespace titian
{
struct TracingMesh
{
    Vector<kl::Triangle> triangles;
    kl::AABB aabb;

    void compute_aabb();
};
}

namespace titian
{
struct TracingTextureCube
{
    kl::Image faces[6];

    void load_cube( kl::Image const& image );
    void load_2D( kl::Image const& image );

    RGB sample( Float3 const& direction ) const;
};
}

namespace titian
{
struct TracingMaterial
{
    float texture_blend = 0.0f;
    float reflectivity_factor = 0.0f;
    float refraction_index = 1.0f;
    Float3 color = kl::colors::WHITE;
    Texture* color_texture = nullptr;
    Texture* normal_texture = nullptr;
    Texture* roughness_texture = nullptr;
};
}

namespace titian
{
struct TracingEntity
{
    TracingMesh mesh;
    TracingMaterial material;
};
}

namespace titian
{
struct TracingCamera
{
    Float3 position;
    Float4x4 inv_mat;
    bool wireframe = false;
    Float4 background;
    Opt<TracingTextureCube> skybox;

    RGB sample_background( Float3 const& direction ) const;
};
}

namespace titian
{
struct TracingAmbient
{
    Float3 color;
};
}

namespace titian
{
struct TracingDirectional
{
    Float3 direction;
    Float3 color;
    float point_size;
};
}

namespace titian
{
struct TracingPayload
{
    TracingEntity const& entity;
    kl::Triangle const& triangle;
    Float3 intersect;
};
}

namespace titian
{
struct TracingScene
{
    List<TracingEntity> entities;
    Opt<TracingCamera> camera;
    Opt<TracingAmbient> ambient;
    Opt<TracingDirectional> directional;

    TracingScene( Scene& scene, Int2 resolution );

    Opt<TracingPayload> trace( kl::Ray const& ray, kl::Triangle const* blacklist ) const;

private:
    Opt<TracingEntity> convert_entity( Scene const& scene, Entity const& entity );
    TracingMesh convert_mesh( Scene const& scene, Mesh const& mesh, Float4x4 const& matrix );
    kl::Vertex convert_vertex( Vertex const& vertex, Float4x4 const& matrix );
    TracingMesh convert_skel_mesh( Scene const& scene, Mesh const& mesh, Float4x4 const& model_matrix, Vector<Float4x4> const& bone_matrices );
    kl::Vertex convert_skel_vertex( Vertex const& vertex, Float4x4 const& model_matrix, Vector<Float4x4> const& bone_matrices );
    TracingMaterial convert_material( Scene const& scene, Material const& material );
    Opt<TracingTextureCube> convert_texture_cube( Texture* texture );
};
}

namespace titian
{
struct Tracing
{
    static inline bool GPU_TRACER = false;
    static inline int DEPTH_LIMIT = 5;
    static inline int ACCUMULATION_LIMIT = 10;
    static inline Int2 RESOLUTION = kl::SCREEN_SIZE;

    static void render( Scene& scene );
};
}
