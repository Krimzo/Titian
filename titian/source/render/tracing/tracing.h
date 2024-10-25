#pragma once

#include "scene/scene.h"


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
		kl::AABB aabb;
		Vector<kl::Triangle> triangles;
		TracingMaterial material;
	};
}

namespace titian {
	struct TracingScene
	{
		struct CameraData
		{
			Float3 position;
			Float4x4 inv_mat;
			bool wireframe = false;
			Float4 background;
			Optional<TracingTextureCube> skybox;

			RGB sample_background(const Float3& direction) const;
		};

		struct AmbientData
		{
			Float3 color;
		};

		struct DirectionalData
		{
			Float3 direction;
			Float3 color;
		};

		struct TracePayload
		{
			TracingEntity& entity;
			kl::Triangle& triangle;
			Float3 intersect;
		};

		Vector<Ref<TracingEntity>> entities;
		Optional<CameraData> camera_data;
		Optional<AmbientData> ambient_data;
		Optional<DirectionalData> directional_data;

		Optional<TracePayload> trace(const kl::Ray& ray, const kl::Triangle* blacklist) const;
	};
}

namespace titian {
	struct Tracing
	{
		static constexpr int MAX_DEPTH = 4;

		static void render(const Scene& scene, Int2 resolution);

	private:
		static void render_scene(const kl::Window& window, const TracingScene& tracing_scene, kl::Image& target);
		static void render_section(const TracingScene& tracing_scene, Int2 min_point, Int2 max_point, kl::Image& target);
		static RGB render_pixel(const TracingScene& tracing_scene, Float2 ndc);
		static RGB render_ray(const TracingScene& tracing_scene, const kl::Ray& ray, int depth, kl::Triangle* blacklist);

		static void convert_scene(const Scene& scene, Int2 resolution, TracingScene& tracing_scene);
		static Ref<TracingEntity> convert_entity(const Scene& scene, const Entity& entity);
		static Optional<TracingTextureCube> convert_texture_cube(const Texture* texture);
		static TracingMaterial convert_material(const Scene& scene, const Material& material);
		static kl::Vertex convert_vertex(const Float4x4& model_matrix, const Vertex& vertex);

		static void handle_input(kl::Window& window, kl::Image& target);
	};
}
