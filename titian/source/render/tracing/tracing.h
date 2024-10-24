#pragma once

#include "scene/scene.h"


namespace titian {
	struct TracingEntity
	{
		kl::AABB aabb;
		Vector<kl::Triangle> triangles;
		const Material& material;

		TracingEntity(const Material& material);
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
			Texture* skybox = nullptr;
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
			Float3 intersect;
		};

		Vector<Ref<TracingEntity>> entities;
		Optional<CameraData> camera_data;
		Optional<AmbientData> ambient_data;
		Optional<DirectionalData> directional_data;

		Optional<TracePayload> trace(const kl::Ray& ray) const;
	};
}

namespace titian {
	struct Tracing
	{
		static void render(const Scene& scene, Int2 resolution);

	private:
		static void render_scene(const TracingScene& tracing_scene, kl::Image& target);
		static void render_section(const TracingScene& tracing_scene, Int2 min_point, Int2 max_point, kl::Image& target);
		static RGB render_pixel(const TracingScene& tracing_scene, Float2 ndc);

		static void convert_scene(const Scene& scene, Int2 resolution, TracingScene& tracing_scene);
		static Ref<TracingEntity> convert_entity(const Scene& scene, const Entity& entity);
		static kl::Vertex convert_vertex(const Float4x4& model_matrix, const Vertex& vertex);
	};
}
