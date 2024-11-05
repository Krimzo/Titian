#pragma once

#include "scene/scene.h"


namespace titian {
	struct TracingMesh
	{
		Vector<kl::Triangle> triangles;
		kl::AABB aabb;

		void compute_aabb();
	};
}

namespace titian {
	struct TracingTextureCube
	{
		kl::Image faces[6];

		void load_cube(const kl::Image& image);
		void load_2D(const kl::Image& image);

		RGB sample(const Float3& direction) const;
	};
}

namespace titian {
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

namespace titian {
	struct TracingEntity
	{
		TracingMesh mesh;
		TracingMaterial material;
	};
}

namespace titian {
	struct TracingCamera
	{
		Float3 position;
		Float4x4 inv_mat;
		bool wireframe = false;
		Float4 background;
		Opt<TracingTextureCube> skybox;

		RGB sample_background(const Float3& direction) const;
	};
}

namespace titian {
	struct TracingAmbient
	{
		Float3 color;
	};
}

namespace titian {
	struct TracingDirectional
	{
		Float3 direction;
		Float3 color;
		float point_size;
	};
}

namespace titian {
	struct TracingPayload
	{
		const TracingEntity& entity;
		const kl::Triangle& triangle;
		Float3 intersect;
	};
}

namespace titian {
	struct TracingScene
	{
		List<TracingEntity> entities;
		Opt<TracingCamera> camera;
		Opt<TracingAmbient> ambient;
		Opt<TracingDirectional> directional;

		TracingScene(const Scene& scene, Int2 resolution);

		Opt<TracingPayload> trace(const kl::Ray& ray, const kl::Triangle* blacklist) const;

	private:
		Opt<TracingEntity> convert_entity(const Scene& scene, const Entity& entity);
		TracingMesh convert_mesh(const Scene& scene, const Mesh& mesh, const Float4x4& matrix);
		kl::Vertex convert_vertex(const Vertex& vertex, const Float4x4& matrix);
		TracingMesh convert_skel_mesh(const Scene& scene, const Mesh& mesh, const Float4x4& model_matrix, const Vector<Float4x4>& bone_matrices);
		kl::Vertex convert_skel_vertex(const Vertex& vertex, const Float4x4& model_matrix, const Vector<Float4x4>& bone_matrices);
		TracingMaterial convert_material(const Scene& scene, const Material& material);
		Opt<TracingTextureCube> convert_texture_cube(const Texture* texture);
	};
}

namespace titian {
	struct Tracing
	{
		static inline bool GPU_TRACER = false;
		static inline int DEPTH_LIMIT = 5;
		static inline int ACCUMULATION_LIMIT = 10;
		static inline Int2 RESOLUTION = kl::SCREEN_SIZE;

		static void render(const Scene& scene);
	};
}
