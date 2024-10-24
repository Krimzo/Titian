#include "titian.h"


titian::TracingEntity::TracingEntity(const Material& material)
	: material(material)
{}

titian::Optional<titian::TracingScene::TracePayload> titian::TracingScene::trace(const kl::Ray& ray) const
{
	Float3 intersection;
	float min_distance = std::numeric_limits<float>::infinity();
	Optional<TracingScene::TracePayload> result;
	for (const auto& entity : entities) {
		if (!ray.intersect_aabb(entity->aabb, &intersection))
			continue;

		const float distance = (intersection - ray.origin).length();
		if (distance >= min_distance)
			continue;

		for (const auto& triangle : entity->triangles) {
			if (!ray.intersect_triangle(triangle, &intersection))
				continue;

			const float distance = (intersection - ray.origin).length();
			if (distance >= min_distance)
				continue;

			min_distance = distance;
			result.emplace(*entity, intersection);
		}
	}
	return result;
}

void titian::Tracing::render(const Scene& scene, const Int2 resolution)
{
	TracingScene tracing_scene;
	convert_scene(scene, resolution, tracing_scene);

	kl::Image target{ resolution };
	std::future<void> render_task = std::async(std::launch::async, render_scene,
		std::ref(tracing_scene), std::ref(target));

	kl::Window window{ "Titian Raytracer" };
	window.set_dark_mode(true);
	window.set_icon("package/textures/editor_icon.ico");
	window.maximize();

	kl::Image frame;
	while (window.process()) {
		frame = target;
		frame.resize_scaled(window.size());
		window.draw_image(frame);
	}
}

void titian::Tracing::render_scene(const TracingScene& tracing_scene, kl::Image& target)
{
	const Int2 target_size = target.size();
	const int core_count = kl::CPU_CORE_COUNT;
	const int square_size = kl::clamp(kl::min(target_size.x, target_size.y) / core_count, 8, 256);
	const Int2 square_counts = {
		(int) ceil(target_size.x / (float) square_size),
		(int) ceil(target_size.y / (float) square_size),
	};
	Vector<std::future<void>> tasks;
	tasks.reserve((size_t) square_counts.x * square_counts.y);
	std::atomic_int32_t working_count = 0;
	for (int y = 0; y < square_counts.y; y++) {
		for (int x = 0; x < square_counts.x; x++) {
			while (working_count >= core_count) {
				std::this_thread::yield();
			}
			tasks.push_back( std::async(std::launch::async, [&, x, y]
			{
				render_section(tracing_scene,
					Int2(x, y) * square_size,
					Int2(x + 1, y + 1) * square_size,
					target);
				--working_count;
			}) );
			++working_count;
		}
	}
	tasks.clear();
}

void titian::Tracing::render_section(const TracingScene& tracing_scene, const Int2 min_point, const Int2 max_point, kl::Image& target)
{
	for (int y = kl::max(min_point.y, 0); y < kl::min(max_point.y, target.height()); y++) {
		for (int x = kl::max(min_point.x, 0); x < kl::min(max_point.x, target.width()); x++) {
			const Float2 ndc = {
				float(x) / (target.width() - 1) * 2.0f - 1.0f,
				float(target.height() - 1 - y) / (target.height() - 1) * 2.0f - 1.0f,
			};
			target[{ x, y }] = render_pixel(tracing_scene, ndc);
		}
	}
}

titian::RGB titian::Tracing::render_pixel(const TracingScene& tracing_scene, const Float2 ndc)
{
	if (!tracing_scene.camera_data)
		return {};

	const TracingScene::CameraData& camera_data = *tracing_scene.camera_data;
	if (camera_data.wireframe)
		return camera_data.background;

	const kl::Ray ray{ camera_data.position, camera_data.inv_mat, ndc };
	Optional<TracingScene::TracePayload> payload = tracing_scene.trace(ray);
	if (!payload)
		return camera_data.background;

	const TracingEntity& entity = payload->entity;
	return entity.material.color;
}

void titian::Tracing::convert_scene(const Scene& scene, const Int2 resolution, TracingScene& tracing_scene)
{
	std::mutex lock;
	std::for_each(std::execution::par, scene.entities().begin(), scene.entities().end(),
		[&](const Pair<String, Ref<Entity>>& pair)
	{
		const Entity& entity = *pair.second;
		Ref<TracingEntity> tracing_entity = convert_entity(scene, entity);
		if (!tracing_entity)
			return;

		lock.lock();
		tracing_scene.entities.push_back(std::move(tracing_entity));
		lock.unlock();
	});
	if (Camera* camera = dynamic_cast<Camera*>(scene.helper_get_entity(scene.main_camera_name))) {
		const float old_ar = camera->aspect_ratio;
		camera->update_aspect_ratio(resolution);
		tracing_scene.camera_data.emplace(
			camera->position(),
			kl::inverse(camera->camera_matrix()),
			camera->render_wireframe,
			camera->background,
			scene.helper_get_texture(camera->skybox_texture_name)
		);
		camera->aspect_ratio = old_ar;
	}
	if (AmbientLight* ambient = dynamic_cast<AmbientLight*>(scene.helper_get_entity(scene.main_ambient_light_name))) {
		tracing_scene.ambient_data.emplace(
			ambient->color
		);
	}
	if (DirectionalLight* directional = dynamic_cast<DirectionalLight*>(scene.helper_get_entity(scene.main_directional_light_name))) {
		tracing_scene.directional_data.emplace(
			directional->direction(),
			directional->color
		);
	}
}

titian::Ref<titian::TracingEntity> titian::Tracing::convert_entity(const Scene& scene, const Entity& entity)
{
	const Animation* animation = scene.helper_get_animation(entity.animation_name);
	if (!animation)
		return {};

	const Mesh* mesh = animation->get_mesh(scene, Layers::get<AppLayer>().timer.elapsed());
	if (!mesh)
		return {};

	if (mesh->topology != MeshTopology::TRIANGLES || mesh->render_wireframe)
		return {};

	const Material* material = scene.helper_get_material(entity.material_name);
	if (!material)
		return {};

	Ref result = new TracingEntity(*material);
	kl::Float3 min_point{ +std::numeric_limits<float>::infinity() };
	kl::Float3 max_point{ -std::numeric_limits<float>::infinity() };

	const Float4x4 model_matrix = entity.model_matrix();
	const uint64_t triangle_count = mesh->vertices.size() / 3;
	for (uint64_t i = 0; i < triangle_count; i++) {
		kl::Triangle triangle;
		triangle.a = convert_vertex(model_matrix, mesh->vertices[i * 3 + 0]);
		triangle.b = convert_vertex(model_matrix, mesh->vertices[i * 3 + 1]);
		triangle.c = convert_vertex(model_matrix, mesh->vertices[i * 3 + 2]);
		result->triangles.push_back(triangle);

		min_point = kl::min(min_point, triangle.a.position);
		min_point = kl::min(min_point, triangle.b.position);
		min_point = kl::min(min_point, triangle.c.position);

		max_point = kl::max(max_point, triangle.a.position);
		max_point = kl::max(max_point, triangle.b.position);
		max_point = kl::max(max_point, triangle.c.position);
	}

	result->aabb.position = (min_point + max_point) * 0.5f;
	result->aabb.size = (max_point - min_point) * 0.5f;
	return result;
}

kl::Vertex titian::Tracing::convert_vertex(const Float4x4& model_matrix, const Vertex& vertex)
{
	kl::Vertex result;
	result.position = (model_matrix * Float4(vertex.position, 1.0f)).xyz();
	result.normal = kl::normalize((model_matrix * Float4(vertex.normal, 0.0f)).xyz());
	result.uv = vertex.uv;
	return result;
}
