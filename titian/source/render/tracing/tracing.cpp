#include "titian.h"


void titian::TracingMesh::compute_aabb()
{
	kl::Float3 min_point{ +std::numeric_limits<float>::infinity() };
	kl::Float3 max_point{ -std::numeric_limits<float>::infinity() };
	for (const auto& triangle : triangles) {
		min_point = kl::min(min_point, triangle.a.position);
		min_point = kl::min(min_point, triangle.b.position);
		min_point = kl::min(min_point, triangle.c.position);
		max_point = kl::max(max_point, triangle.a.position);
		max_point = kl::max(max_point, triangle.b.position);
		max_point = kl::max(max_point, triangle.c.position);
	}
	aabb.position = (min_point + max_point) * 0.5f;
	aabb.size = (max_point - min_point) * 0.5f;
}

void titian::TracingTextureCube::load_cube(const kl::Image& image)
{
	const int part_size = image.width() / 4;
	faces[0] = image.rectangle(Int2(2, 1) * part_size, Int2(3, 2) * part_size);
	faces[1] = image.rectangle(Int2(0, 1) * part_size, Int2(1, 2) * part_size);
	faces[2] = image.rectangle(Int2(1, 0) * part_size, Int2(2, 1) * part_size);
	faces[3] = image.rectangle(Int2(1, 2) * part_size, Int2(2, 3) * part_size);
	faces[4] = image.rectangle(Int2(1, 1) * part_size, Int2(2, 2) * part_size);
	faces[5] = image.rectangle(Int2(3, 1) * part_size, Int2(4, 2) * part_size);
}

void titian::TracingTextureCube::load_2D(const kl::Image& image)
{
	for (auto& face : faces) {
		face = image;
	}
}

titian::RGB titian::TracingTextureCube::sample(const Float3& direction) const
{
	const auto [x, y, z] = kl::normalize(direction);
	const auto [ax, ay, az] = kl::abs(Float3(x, y, z));
	auto [u, v] = Float2();
	
	int face_index;
	if (ax >= ay && ax >= az) {
		if (x > 0) { face_index = 0; u = -z / ax; v = -y / ax; }
		else       { face_index = 1; u =  z / ax; v = -y / ax; }
	}
	else if (ay >= ax && ay >= az) {
		if (y > 0) { face_index = 2; u =  x / ay; v =  z / ay; }
		else       { face_index = 3; u =  x / ay; v = -z / ay; }
	}
	else {
		if (z > 0) { face_index = 4; u =  x / az; v = -y / az; }
		else       { face_index = 5; u = -x / az; v = -y / az; }
	}

	u = u * 0.5f + 0.5f;
	v = v * 0.5f + 0.5f;
	return faces[face_index].sample({ u, v });
}

titian::RGB titian::TracingCamera::sample_background(const Float3& direction) const
{
	if (skybox) {
		return skybox->sample(direction);
	}
	return background;
}

titian::Opt<titian::TracingPayload> titian::TracingScene::trace(const kl::Ray& ray, const kl::Triangle* blacklist) const
{
	Float3 intersection;
	float min_distance = std::numeric_limits<float>::infinity();
	Opt<TracingPayload> result;
	for (const auto& entity : entities) {
		if (!ray.intersect_aabb(entity.mesh.aabb, &intersection))
			continue;

		const float distance = (intersection - ray.origin).length();
		if (distance >= min_distance)
			continue;
		
		for (const auto& triangle : entity.mesh.triangles) {
			if (&triangle == blacklist)
				continue;

			if (!ray.intersect_triangle(triangle, &intersection))
				continue;

			const float distance = (intersection - ray.origin).length();
			if (distance >= min_distance)
				continue;

			min_distance = distance;
			result.emplace(entity, triangle, intersection);
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
			if (!window.active())
				goto loop_end;

			tasks.push_back( std::async(std::launch::async, [&, x, y]
			{
				render_section(tracing_scene,
					Int2(x, y) * square_size,
					Int2(square_size),
					target);
				--working_count;
			}) );
			++working_count;
		}
	}
loop_end:
	tasks.clear();
}

void titian::Tracing::render_section(const TracingScene& tracing_scene, const Int2 top_left, const Int2 size, kl::Image& target)
{
	const Int2 incl_start = kl::max(top_left, Int2(0));
	const Int2 excl_end = kl::min(top_left + size, target.size());
	for (int y = incl_start.y; y < excl_end.y; y++) {
		for (int x = incl_start.x; x < excl_end.x; x++) {
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
	if (!tracing_scene.camera)
		return RGB{};

	const TracingCamera& camera = *tracing_scene.camera;
	const kl::Ray ray{ camera.position, camera.inv_mat, ndc };
	if (camera.wireframe)
		return camera.sample_background(ray.direction());

	Float3 color;
	for (int i = 0; i < ACCUMULATION_LIMIT; i++) {
		color += trace_ray(tracing_scene, ray, 0, nullptr);
	}
	return color * (1.0f / ACCUMULATION_LIMIT);
}

titian::Float3 titian::Tracing::trace_ray(const TracingScene& tracing_scene, const kl::Ray& ray, const int depth, const kl::Triangle* blacklist)
{
	if (depth > DEPTH_LIMIT)
		return {};

	Opt<TracingPayload> payload = tracing_scene.trace(ray, blacklist);
	if (!payload) {
		if (tracing_scene.directional) {
			auto& directional = *tracing_scene.directional;
			const kl::Sphere sun_sphere{ ray.origin - directional.direction, directional.point_size };
			if (ray.intersect_sphere(sun_sphere, nullptr)) {
				return directional.color;
			}
		}
		return tracing_scene.camera->sample_background(ray.direction());
	}

	auto& material = payload->entity.material;
	const Float3 interp_weights = payload->triangle.weights(payload->intersect);
	const kl::Vertex interp_vertex = payload->triangle.interpolate_self(interp_weights);

	Float3 normal = interp_vertex.normal;
	if (material.normal_texture) {
		Float3 tex_norm = material.normal_texture->image.sample(interp_vertex.uv);
		tex_norm = kl::normalize(tex_norm * 2.0f - Float3(1.0f));
		normal = kl::normalize(interp_vertex.normal - tex_norm);
	}
	if (kl::dot(-ray.direction(), normal) < 0.0f) {
		normal = -normal;
	}

	Float3 light;
	if (material.reflectivity_factor >= 0.0f) {
		const Float3 random_dir = kl::normalize(kl::random::gen_float3(-1.0f, 1.0f));
		const float random_infl = 1.0f - kl::clamp(material.reflectivity_factor, 0.0f, 1.0f);
		const Float3 random_norm = kl::normalize(normal + random_dir * random_infl);
		const Float3 reflect_dir = kl::reflect(ray.direction(), random_norm);
		const kl::Ray reflection_ray{ payload->intersect, reflect_dir };
		light += trace_ray(tracing_scene, reflection_ray, depth + 1, &payload->triangle);
	}
	else {
		const Float3 random_dir = kl::normalize(kl::random::gen_float3(-1.0f, 1.0f));
		const float random_infl = 1.0f - kl::clamp(-material.reflectivity_factor, 0.0f, 1.0f);
		const Float3 random_norm = kl::normalize(normal + random_dir * random_infl);
		const Float3 refract_dir = kl::refract(ray.direction(), random_norm, 1.0f / material.refraction_index );
		const kl::Ray refraction_ray{ payload->intersect, refract_dir };
		light += trace_ray(tracing_scene, refraction_ray, depth + 1, &payload->triangle);
	}

	Float3 color = material.color;
	if (material.color_texture) {
		Float3 tex_col = material.color_texture->image.sample(interp_vertex.uv);
		color = kl::lerp(material.texture_blend, color, tex_col);
	}
	return color * light;
}

void titian::Tracing::convert_scene(const Scene& scene, const Int2 resolution, TracingScene& tracing_scene)
{
	std::mutex lock;
	std::for_each(std::execution::par, scene.entities().begin(), scene.entities().end(),
		[&](const Pair<String, Ref<Entity>>& pair)
	{
		const Entity& entity = *pair.second;
		const Opt<TracingEntity> tracing_entity = convert_entity(scene, entity);
		if (!tracing_entity)
			return;

		lock.lock();
		tracing_scene.entities.push_back(*tracing_entity);
		lock.unlock();
	});
	if (Camera* camera = dynamic_cast<Camera*>(scene.helper_get_entity(scene.main_camera_name))) {
		Texture* skybox = scene.helper_get_texture(camera->skybox_texture_name);
		const float old_ar = camera->aspect_ratio;
		camera->update_aspect_ratio(resolution);
		tracing_scene.camera.emplace(
			camera->position(),
			kl::inverse(camera->camera_matrix()),
			camera->render_wireframe,
			camera->background,
			convert_texture_cube(skybox)
		);
		camera->aspect_ratio = old_ar;
	}
	if (AmbientLight* ambient = dynamic_cast<AmbientLight*>(scene.helper_get_entity(scene.main_ambient_light_name))) {
		tracing_scene.ambient.emplace(
			ambient->color
		);
	}
	if (DirectionalLight* directional = dynamic_cast<DirectionalLight*>(scene.helper_get_entity(scene.main_directional_light_name))) {
		tracing_scene.directional.emplace(
			directional->direction(),
			directional->color,
			directional->point_size
		);
	}
}

titian::Opt<titian::TracingEntity> titian::Tracing::convert_entity(const Scene& scene, const Entity& entity)
{
	const Animation* animation = scene.helper_get_animation(entity.animation_name);
	if (!animation)
		return std::nullopt;

	const Mesh* mesh = animation->get_mesh(scene, AppLayer::get().timer.elapsed());
	if (!mesh)
		return std::nullopt;

	if (mesh->topology != MeshTopology::TRIANGLES || mesh->render_wireframe)
		return std::nullopt;

	const Material* material = scene.helper_get_material(entity.material_name);
	if (!material)
		return std::nullopt;

	TracingEntity result;
	if (animation->animation_type == AnimationType::SKELETAL) {
		result.mesh = convert_skel_mesh(scene, *mesh, entity.model_matrix(), animation->matrices());
	}
	else {
		result.mesh = convert_mesh(scene, *mesh, entity.model_matrix());
	}
	result.material = convert_material(scene, *material);
	return result;
}

titian::TracingMesh titian::Tracing::convert_mesh(const Scene& scene, const Mesh& mesh, const Float4x4& matrix)
{
	TracingMesh result;
	result.triangles.resize(mesh.vertices.size() / 3);
	for (size_t i = 0; i < result.triangles.size(); i++) {
		auto& triangle = result.triangles[i];
		triangle.a = convert_vertex(mesh.vertices[i * 3 + 0], matrix);
		triangle.b = convert_vertex(mesh.vertices[i * 3 + 1], matrix);
		triangle.c = convert_vertex(mesh.vertices[i * 3 + 2], matrix);
	}
	result.compute_aabb();
	return result;
}

kl::Vertex titian::Tracing::convert_vertex(const Vertex& vertex, const Float4x4& matrix)
{
	kl::Vertex result;
	result.position = (matrix * Float4(vertex.position, 1.0f)).xyz();
	result.normal = kl::normalize((matrix * Float4(vertex.normal, 0.0f)).xyz());
	result.uv = vertex.uv;
	return result;
}

titian::TracingMesh titian::Tracing::convert_skel_mesh(const Scene& scene, const Mesh& mesh, const Float4x4& model_matrix, const Vector<Float4x4>& bone_matrices)
{
	TracingMesh result;
	result.triangles.resize(mesh.vertices.size() / 3);
	for (size_t i = 0; i < result.triangles.size(); i++) {
		auto& triangle = result.triangles[i];
		triangle.a = convert_skel_vertex(mesh.vertices[i * 3 + 0], model_matrix, bone_matrices);
		triangle.b = convert_skel_vertex(mesh.vertices[i * 3 + 1], model_matrix, bone_matrices);
		triangle.c = convert_skel_vertex(mesh.vertices[i * 3 + 2], model_matrix, bone_matrices);
	}
	result.compute_aabb();
	return result;
}

kl::Vertex titian::Tracing::convert_skel_vertex(const Vertex& vertex, const Float4x4& model_matrix, const Vector<Float4x4>& bone_matrices)
{
	const Float4x4 bone_mat =
		(vertex.bone_indices[0] < bone_matrices.size() ? bone_matrices[vertex.bone_indices[0]] : Float4x4{}) * vertex.bone_weights[0] +
		(vertex.bone_indices[1] < bone_matrices.size() ? bone_matrices[vertex.bone_indices[1]] : Float4x4{}) * vertex.bone_weights[1] +
		(vertex.bone_indices[2] < bone_matrices.size() ? bone_matrices[vertex.bone_indices[2]] : Float4x4{}) * vertex.bone_weights[2] +
		(vertex.bone_indices[3] < bone_matrices.size() ? bone_matrices[vertex.bone_indices[3]] : Float4x4{}) * vertex.bone_weights[3];
	return convert_vertex(vertex, model_matrix * bone_mat);
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

titian::Opt<titian::TracingTextureCube> titian::Tracing::convert_texture_cube(const Texture* texture)
{
	if (!texture)
		return std::nullopt;

	Opt<TracingTextureCube> result = TracingTextureCube{};
	const kl::Image& image = texture->image;
	if (texture->is_cube() && image.width() % 4 == 0 && image.height() % 3 == 0) {
		result->load_cube(image);
	}
	else {
		result->load_2D(image);
	}
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
