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
} // namespace titian

namespace titian
{
struct TracingTextureCube
{
    kl::Image faces[6];

    void load_cube(kl::Image const& image);
    void load_2D(kl::Image const& image);

    RGB sample(float3 const& direction) const;
};
} // namespace titian

namespace titian
{
struct TracingMaterial
{
    float texture_blend = 0.0f;
    float reflectivity_factor = 0.0f;
    float refraction_index = 1.0f;
    float3 color = kl::colors::WHITE;
    Texture* color_texture = nullptr;
    Texture* normal_texture = nullptr;
    Texture* roughness_texture = nullptr;
};
} // namespace titian

namespace titian
{
struct TracingEntity
{
    TracingMesh mesh;
    TracingMaterial material;
};
} // namespace titian

namespace titian
{
struct TracingCamera
{
    float3 position;
    float4x4 inv_mat;
    bool wireframe = false;
    float4 background;
    Opt<TracingTextureCube> skybox;

    RGB sample_background(float3 const& direction) const;
};
} // namespace titian

namespace titian
{
struct TracingAmbient
{
    float3 color;
};
} // namespace titian

namespace titian
{
struct TracingDirectional
{
    float3 direction;
    float3 color;
    float point_size;
};
} // namespace titian

namespace titian
{
struct TracingPayload
{
    TracingEntity const& entity;
    kl::Triangle const& triangle;
    float3 intersect;
};
} // namespace titian

namespace titian
{
struct TracingScene
{
    List<TracingEntity> entities;
    Opt<TracingCamera> camera;
    Opt<TracingAmbient> ambient;
    Opt<TracingDirectional> directional;

    TracingScene(Scene& scene, int2 resolution);

    Opt<TracingPayload> trace(kl::Ray const& ray, kl::Triangle const* blacklist) const;

  private:
    Opt<TracingEntity> convert_entity(Scene const& scene, Entity const& entity);
    TracingMesh convert_mesh(Scene const& scene, Mesh const& mesh, float4x4 const& matrix);
    kl::Vertex convert_vertex(Vertex const& vertex, float4x4 const& matrix);
    TracingMesh convert_skel_mesh(Scene const& scene, Mesh const& mesh, float4x4 const& model_matrix,
                                  Vector<float4x4> const& bone_matrices);
    kl::Vertex convert_skel_vertex(Vertex const& vertex, float4x4 const& model_matrix,
                                   Vector<float4x4> const& bone_matrices);
    TracingMaterial convert_material(Scene const& scene, Material const& material);
    Opt<TracingTextureCube> convert_texture_cube(Texture* texture);
};
} // namespace titian

namespace titian
{
struct Tracing
{
    static inline bool GPU_TRACER = false;
    static inline int DEPTH_LIMIT = 5;
    static inline int ACCUMULATION_LIMIT = 10;
    static inline int2 RESOLUTION = kl::SCREEN_SIZE;

    static void render(Scene& scene);
};
} // namespace titian
