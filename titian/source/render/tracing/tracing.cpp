#include "titian.h"


titian::Optional<titian::TracingScene::TracePayload> titian::TracingScene::trace(const kl::Ray& ray, const kl::Triangle* blacklist) const
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

		for (auto& triangle : entity->triangles) {
			if (&triangle == blacklist)
				continue;

			if (!ray.intersect_triangle(triangle, &intersection))
				continue;

			const float distance = (intersection - ray.origin).length();
			if (distance >= min_distance)
				continue;

			min_distance = distance;
			result.emplace(*entity, triangle, intersection);
		}
	}
	return result;
}

void titian::Tracing::render(const Scene& scene, const Int2 resolution)
{
	TracingScene tracing_scene;
	convert_scene(scene, resolution, tracing_scene);

	kl::Window window{ "Titian Raytracer" };
	window.set_dark_mode(true);
	window.set_icon("package/textures/editor_icon.ico");
	window.maximize();

	kl::Image target{ resolution };
	std::future<void> render_task = std::async(std::launch::async, render_scene,
		std::ref(window), std::ref(tracing_scene), std::ref(target));

	kl::Image frame;
	while (window.process()) {
		handle_input(window, target);
		frame = target;
		frame.resize_scaled(window.size());
		window.draw_image(frame);
	}
}

void titian::Tracing::render_scene(const kl::Window& window, const TracingScene& tracing_scene, kl::Image& target)
{
	const Int2 target_size = target.size();
	const int core_count = kl::CPU_CORE_COUNT;
	const int square_size = kl::clamp(kl::min(target_size.x, target_size.y) / core_count, 8, 256);
	const Int2 square_counts = {
		(int) ceil(target_size.x / (float) square_size),
		(int) ceil(target_size.y / (float) square_size),
	};
	std::atomic_int32_t working_count = 0;
	Vector<std::future<void>> tasks;
	tasks.reserve((size_t) square_counts.x * square_counts.y);
	for (int y = 0; y < square_counts.y; y++) {
		for (int x = 0; x < square_counts.x; x++) {
			while (working_count >= core_count) {
				std::this_thread::yield();
			}
			if (!window.is_open())
				goto loop_end;

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
loop_end:
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
		return RGB{};

	const TracingScene::CameraData& camera_data = *tracing_scene.camera_data;
	if (camera_data.wireframe)
		return camera_data.background;

	const kl::Ray ray{ camera_data.position, camera_data.inv_mat, ndc };
	return render_ray(tracing_scene, ray, 0, nullptr);
}

titian::RGB titian::Tracing::render_ray(const TracingScene& tracing_scene, const kl::Ray& ray, const int depth, kl::Triangle* blacklist)
{
	if (depth > MAX_DEPTH)
		return tracing_scene.camera_data->background;

	Optional<TracingScene::TracePayload> payload = tracing_scene.trace(ray, blacklist);
	if (!payload)
		return tracing_scene.camera_data->background;

	auto& material = payload->entity.material;

	const Float3 interp_weights = payload->triangle.weights(payload->intersect);
	const kl::Vertex interp_vertex = payload->triangle.interpolate_self(interp_weights);

	Float3 color = material.color;
	Float3 normal = interp_vertex.normal;

	if (material.color_texture) {
		Float3 tex_col = material.color_texture->image.sample(interp_vertex.uv);
		color.x = kl::lerp(material.texture_blend, color.x, tex_col.x);
		color.y = kl::lerp(material.texture_blend, color.y, tex_col.y);
		color.z = kl::lerp(material.texture_blend, color.z, tex_col.z);
	}
	if (material.normal_texture) {
		Float3 tex_norm = material.normal_texture->image.sample(interp_vertex.uv);
		tex_norm = kl::normalize(tex_norm * 2.0f - Float3(1.0f));
		normal = kl::normalize(interp_vertex.normal - tex_norm);
	}

	Float3 light;
	if (tracing_scene.ambient_data) {
		auto& ambient = *tracing_scene.ambient_data;
		float ambient_factor = kl::max(kl::dot(interp_vertex.normal, normal), 0.0f);
		light = ambient.color * ambient_factor;
	}
	if (tracing_scene.directional_data) {
		auto& directional = *tracing_scene.directional_data;
		const kl::Ray shadow_ray{ payload->intersect, -directional.direction };
		if (!tracing_scene.trace(shadow_ray, &payload->triangle)) {
			float diffuse_factor = kl::max(kl::dot(-directional.direction, normal), 0.0f);
			Float3 diffse = directional.color * diffuse_factor;
			light = kl::max(light, diffse);
		}
	}
	color *= light;

	Float3 ref_normal = interp_vertex.normal;
	if (kl::dot(-ray.direction(), ref_normal) < 0.0f) {
		ref_normal = -ref_normal;
	}

	if (material.reflectivity_factor > 0.0f) {
		const kl::Ray reflection_ray{ payload->intersect, kl::reflect(ray.direction(), ref_normal) };
		const Float3 reflect_color = render_ray(tracing_scene, reflection_ray, depth + 1, &payload->triangle);
		const float reflectivity = kl::min(material.reflectivity_factor, 1.0f);
		color.x = kl::lerp(reflectivity, color.x, reflect_color.x);
		color.y = kl::lerp(reflectivity, color.y, reflect_color.y);
		color.z = kl::lerp(reflectivity, color.z, reflect_color.z);
	}
	else if (material.reflectivity_factor < 0.0f) {
		const kl::Ray refraction_ray{ payload->intersect, kl::refract(ray.direction(), ref_normal, 1.0f / material.refraction_index) };
		const Float3 refract_color = render_ray(tracing_scene, refraction_ray, depth + 1, &payload->triangle);
		const float refractivity = kl::min(-material.reflectivity_factor, 1.0f);
		color.x = kl::lerp(refractivity, color.x, refract_color.x);
		color.y = kl::lerp(refractivity, color.y, refract_color.y);
		color.z = kl::lerp(refractivity, color.z, refract_color.z);
	}

	return color;
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

	Ref result = new TracingEntity();
	result->material = convert_material(scene, *material);

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

titian::TracingMaterial titian::Tracing::convert_material(const Scene& scene, const Material& material)
{
	TracingMaterial result;
	result.texture_blend = material.texture_blend;
	result.reflectivity_factor = material.reflectivity_factor;
	result.refraction_index = material.refraction_index;
	result.color = material.color.xyz();
	result.color_texture = scene.helper_get_texture(material.color_texture_name);
	result.normal_texture = scene.helper_get_texture(material.normal_texture_name);
	result.roughness_texture = scene.helper_get_texture(material.roughness_texture_name);
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

void titian::Tracing::handle_input(kl::Window& window, kl::Image& target)
{
	if (window.keyboard.esc) {
		window.close();
	}
	if (window.keyboard.ctrl && window.keyboard.s.pressed()) {
		if (auto path = kl::choose_file(true, { { "Images", ".png" } })) {
			if (!path->ends_with(".png")) {
				path->append(".png");
			}
			target.save_to_file(path.value(), kl::ImageType::PNG);
		}
	}
}
