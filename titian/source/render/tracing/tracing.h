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

		Opt<TracingPayload> trace(const kl::Ray& ray, const kl::Triangle* blacklist) const;
	};
}

namespace titian {
	struct Tracing
	{
		static inline int DEPTH_LIMIT = 5;
		static inline int ACCUMULATION_LIMIT = 10;

		static void render(const Scene& scene, Int2 resolution);

	private:
		static void render_scene(const kl::Window& window, const TracingScene& tracing_scene, kl::Image& target);
		static void render_section(const TracingScene& tracing_scene, Int2 top_left, Int2 size, kl::Image& target);
		static RGB render_pixel(const TracingScene& tracing_scene, Float2 ndc);

		static Float3 trace_ray(const TracingScene& tracing_scene, const kl::Ray& ray, int depth, const kl::Triangle* blacklist);

		static void convert_scene(const Scene& scene, Int2 resolution, TracingScene& tracing_scene);
		static Opt<TracingEntity> convert_entity(const Scene& scene, const Entity& entity);

		static TracingMesh convert_mesh(const Scene& scene, const Mesh& mesh, const Float4x4& matrix);
		static kl::Vertex convert_vertex(const Vertex& vertex, const Float4x4& matrix);

		static TracingMesh convert_skel_mesh(const Scene& scene, const Mesh& mesh, const Float4x4& model_matrix, const Vector<Float4x4>& bone_matrices);
		static kl::Vertex convert_skel_vertex(const Vertex& vertex, const Float4x4& model_matrix, const Vector<Float4x4>& bone_matrices);

		static TracingMaterial convert_material(const Scene& scene, const Material& material);
		static Opt<TracingTextureCube> convert_texture_cube(const Texture* texture);

		static void handle_input(kl::Window& window, kl::Image& target);
	};
}
