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

titian::TracingScene::TracingScene(const Scene& scene, const Int2 resolution)
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
		this->entities.push_back(*tracing_entity);
		lock.unlock();
	});
	if (Camera* camera = dynamic_cast<Camera*>(scene.helper_get_entity(scene.main_camera_name))) {
		Texture* skybox = scene.helper_get_texture(camera->skybox_texture_name);
		const float old_ar = camera->aspect_ratio;
		camera->update_aspect_ratio(resolution);
		this->camera.emplace(
			camera->position(),
			kl::inverse(camera->camera_matrix()),
			camera->render_wireframe,
			camera->background,
			convert_texture_cube(skybox)
		);
		camera->aspect_ratio = old_ar;
	}
	if (AmbientLight* ambient = dynamic_cast<AmbientLight*>(scene.helper_get_entity(scene.main_ambient_light_name))) {
		this->ambient.emplace(
			ambient->color
		);
	}
	if (DirectionalLight* directional = dynamic_cast<DirectionalLight*>(scene.helper_get_entity(scene.main_directional_light_name))) {
		this->directional.emplace(
			directional->direction(),
			directional->color,
			directional->point_size
		);
	}
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

titian::Opt<titian::TracingEntity> titian::TracingScene::convert_entity(const Scene& scene, const Entity& entity)
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

titian::TracingMesh titian::TracingScene::convert_mesh(const Scene& scene, const Mesh& mesh, const Float4x4& matrix)
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

kl::Vertex titian::TracingScene::convert_vertex(const Vertex& vertex, const Float4x4& matrix)
{
	kl::Vertex result;
	result.position = (matrix * Float4(vertex.position, 1.0f)).xyz();
	result.normal = kl::normalize((matrix * Float4(vertex.normal, 0.0f)).xyz());
	result.uv = vertex.uv;
	return result;
}

titian::TracingMesh titian::TracingScene::convert_skel_mesh(const Scene& scene, const Mesh& mesh, const Float4x4& model_matrix, const Vector<Float4x4>& bone_matrices)
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

kl::Vertex titian::TracingScene::convert_skel_vertex(const Vertex& vertex, const Float4x4& model_matrix, const Vector<Float4x4>& bone_matrices)
{
	const Float4x4 bone_mat =
		(vertex.bone_indices[0] < bone_matrices.size() ? bone_matrices[vertex.bone_indices[0]] : Float4x4{}) * vertex.bone_weights[0] +
		(vertex.bone_indices[1] < bone_matrices.size() ? bone_matrices[vertex.bone_indices[1]] : Float4x4{}) * vertex.bone_weights[1] +
		(vertex.bone_indices[2] < bone_matrices.size() ? bone_matrices[vertex.bone_indices[2]] : Float4x4{}) * vertex.bone_weights[2] +
		(vertex.bone_indices[3] < bone_matrices.size() ? bone_matrices[vertex.bone_indices[3]] : Float4x4{}) * vertex.bone_weights[3];
	return convert_vertex(vertex, model_matrix * bone_mat);
}

titian::TracingMaterial titian::TracingScene::convert_material(const Scene& scene, const Material& material)
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

titian::Opt<titian::TracingTextureCube> titian::TracingScene::convert_texture_cube(const Texture* texture)
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

void titian::Tracing::render(const Scene& scene)
{
	const TracingScene tracing_scene{ scene, RESOLUTION };

	kl::Window window{ "Titian Raytracer" };
	window.set_dark_mode(true);
	window.set_icon("package/textures/editor_icon.ico");
	window.maximize();

	if (GPU_TRACER) {
		GPUTracer{ window, tracing_scene, RESOLUTION }.render();
	}
	else {
		CPUTracer{ window, tracing_scene, RESOLUTION }.render();
	}
}
