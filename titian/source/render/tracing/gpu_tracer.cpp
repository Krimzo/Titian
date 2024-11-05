#include "titian.h"


titian::GPUTracer::GPUTracer(kl::Window& window, const TracingScene& scene, const Int2 resolution)
	: window(window), scene(scene), m_gpu(window.ptr())
{
	dx::TextureDescriptor tex_desc{};
	tex_desc.Width = resolution.x;
	tex_desc.Height = resolution.y;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

	m_target = m_gpu.create_texture(&tex_desc, nullptr);
	m_target_access_view = m_gpu.create_access_view(m_target, nullptr);
	kl::verify(m_target, "Failed to create tracing target texture");
	kl::verify(m_target_access_view, "Failed to create tracing target access view");

	m_target_copy = m_gpu.create_texture(&tex_desc, nullptr);
	m_target_copy_access_view = m_gpu.create_access_view(m_target_copy, nullptr);
	m_target_copy_shader_view = m_gpu.create_shader_view(m_target_copy, nullptr);
	kl::verify(m_target, "Failed to create tracing target copy texture");
	kl::verify(m_target_copy_access_view, "Failed to create tracing target copy access view");
	kl::verify(m_target_copy_shader_view, "Failed to create tracing target copy shader view");

	m_compute_shader = m_gpu.create_compute_shader(kl::read_file("package/shaders/tracing/tracing_compute.hlsl"));
	m_copy_shader = m_gpu.create_compute_shader(kl::read_file("package/shaders/tracing/tracing_copy.hlsl"));
	m_display_shaders = m_gpu.create_shaders(kl::read_file("package/shaders/tracing/tracing_display.hlsl"));
	kl::verify(m_compute_shader, "Failed to create tracing compute shader");
	kl::verify(m_copy_shader, "Failed to create tracing copy shader");
	kl::verify(m_display_shaders, "Failed to create tracing display shaders");

	m_screen_mesh = m_gpu.create_screen_mesh();
	m_linear_sampler = m_gpu.create_sampler_state(true, false);
	kl::verify(m_screen_mesh, "Failed to create tracing screen mesh");
	kl::verify(m_linear_sampler, "Failed to create tracing linear sampler");

	create_scene_buffers();
}

void titian::GPUTracer::render()
{
	while (window.process()) {
		m_iterations += 1;
		m_timer.update();
		handle_input();
		m_gpu.clear_internal();
		compute_pass();
		copy_pass();
		display_pass();
		m_gpu.swap_buffers(false);
	}
}

void titian::GPUTracer::create_scene_buffers()
{
	struct Mesh
	{
		kl::AABB aabb;
		int triangles_first = 0;
		int triangles_count = 0;
	};
	struct Material
	{
		Float3 color;
		float texture_blend = 0.0f;
		float reflectivity_factor = 0.0f;
		float refraction_index = 0.0f;
	};
	struct Entity
	{
		Mesh mesh;
		Material material;
	};
	using Vertex = kl::Vertex;
	using Triangle = kl::Triangle;

	Vector<Entity> entities;
	Vector<Triangle> triangles;
	for (const auto& tracing_entity : scene.entities) {
		Entity& entity = entities.emplace_back();
		entity.mesh.aabb = tracing_entity.mesh.aabb;
		entity.mesh.triangles_first = (int) triangles.size();
		entity.mesh.triangles_count = (int) tracing_entity.mesh.triangles.size();
		entity.material.color = tracing_entity.material.color;
		entity.material.texture_blend = tracing_entity.material.texture_blend;
		entity.material.reflectivity_factor = tracing_entity.material.reflectivity_factor;
		entity.material.refraction_index = tracing_entity.material.refraction_index;
		triangles.insert(triangles.end(), tracing_entity.mesh.triangles.begin(), tracing_entity.mesh.triangles.end());
	}

	dx::Buffer entities_buffer = m_gpu.create_structured_buffer(entities.data(), (UINT) entities.size(), sizeof(Entity));
	dx::Buffer triangles_buffer = m_gpu.create_structured_buffer(triangles.data(), (UINT) triangles.size(), sizeof(Triangle));
	m_entities_shader_view = m_gpu.create_shader_view(entities_buffer, nullptr);
	m_triangles_shader_view = m_gpu.create_shader_view(triangles_buffer, nullptr);

	if (scene.camera && scene.camera->skybox) {
		auto& faces = scene.camera->skybox->faces;
		dx::Texture sky_texture = m_gpu.create_cube_texture(faces[0], faces[1], faces[2], faces[3], faces[4], faces[5]);
		m_skybox_shader_view = m_gpu.create_shader_view(sky_texture, nullptr);
	}
}

void titian::GPUTracer::handle_input()
{
	if (window.keyboard.esc) {
		window.close();
	}
	if (window.keyboard.ctrl && window.keyboard.s.pressed()) {
		if (auto path = kl::choose_file(true, { { "Images", ".png" } })) {
			if (!path->ends_with(".png")) {
				path->append(".png");
			}
			save_screenshot(path.value());
		}
	}
}

void titian::GPUTracer::save_screenshot(const StringRef& path)
{
	const Int2 resolution = m_gpu.texture_size(m_target);
	dx::Texture staging_texture = m_gpu.create_staging_texture(m_target);
	m_gpu.copy_resource(staging_texture, m_target);
	Vector<Float4> samples_buffer;
	samples_buffer.resize((size_t) resolution.x * resolution.y);
	m_gpu.read_from_texture(samples_buffer.data(), staging_texture, resolution, sizeof(Float4));
	kl::Image image;
	image.resize(resolution);
	const float rec_iter = (1.0f / m_iterations);
	kl::async_for(0, image.pixel_count(), [&](int i)
	{
		Float3 result = samples_buffer[i].xyz() * rec_iter;
		image[i] = Float4{ result, 1.0f };
	});
	image.save_to_file(path, kl::ImageType::PNG);
}

void titian::GPUTracer::compute_pass()
{
	const Int2 resolution = m_gpu.texture_size(m_target);

	m_gpu.bind_compute_shader(m_compute_shader.shader);
	m_gpu.bind_access_view_for_compute_shader(m_target_access_view, 0);
	m_gpu.bind_shader_view_for_compute_shader(m_entities_shader_view, 0);
	m_gpu.bind_shader_view_for_compute_shader(m_triangles_shader_view, 1);
	m_gpu.bind_shader_view_for_compute_shader(m_skybox_shader_view, 2);
	m_gpu.bind_sampler_state_for_compute_shader(m_linear_sampler, 0);

	struct alignas(16) CB
	{
		Float4x4 CAMERA_INV_MAT;
		Float4 CAMERA_BACKGROUND;
		Float3 CAMERA_POSITION;
		int32_t DEPTH_LIMIT;
		Float3 DIRECTIONAL_DIRECTION;
		float CAMERA_IS_WIREFRAME;
		Float3 DIRECTIONAL_COLOR;
		float CAMERA_HAS_SKYBOX;
		float CAMERA_BOUND;
		float DIRECTIONAL_POINT_SIZE;
		float DIRECTIONAL_BOUND;
		float TIME;
	};

	CB cb{};
	cb.DEPTH_LIMIT = Tracing::DEPTH_LIMIT;
	cb.TIME = m_timer.elapsed();
	if (scene.camera) {
		auto& camera = scene.camera.value();
		cb.CAMERA_INV_MAT = camera.inv_mat;
		cb.CAMERA_BACKGROUND = camera.background;
		cb.CAMERA_POSITION = camera.position;
		cb.CAMERA_IS_WIREFRAME = camera.wireframe;
		cb.CAMERA_HAS_SKYBOX = m_skybox_shader_view;
		cb.CAMERA_BOUND = true;
	}
	if (scene.directional) {
		auto& directional = scene.directional.value();
		cb.DIRECTIONAL_DIRECTION = directional.direction;
		cb.DIRECTIONAL_COLOR = directional.color;
		cb.DIRECTIONAL_POINT_SIZE = directional.point_size;
		cb.DIRECTIONAL_BOUND = true;
	}
	m_compute_shader.upload(cb);

	m_gpu.dispatch_compute_shader(resolution.x / 32 + 1, resolution.y / 32 + 1, 1);

	m_gpu.unbind_sampler_state_for_compute_shader(0);
	m_gpu.unbind_shader_view_for_compute_shader(2);
	m_gpu.unbind_shader_view_for_compute_shader(1);
	m_gpu.unbind_shader_view_for_compute_shader(0);
	m_gpu.unbind_access_view_for_compute_shader(0);
	m_gpu.unbind_compute_shader();
}

void titian::GPUTracer::copy_pass()
{
	const Int2 resolution = m_gpu.texture_size(m_target);

	m_gpu.bind_compute_shader(m_copy_shader.shader);
	m_gpu.bind_access_view_for_compute_shader(m_target_access_view, 0);
	m_gpu.bind_access_view_for_compute_shader(m_target_copy_access_view, 1);

	struct alignas(16) CB
	{
		float REC_ITERATIONS;
	};

	CB cb{};
	cb.REC_ITERATIONS = 1.0f / m_iterations;
	m_copy_shader.upload(cb);

	m_gpu.dispatch_compute_shader(resolution.x / 32 + 1, resolution.y / 32 + 1, 1);

	m_gpu.unbind_access_view_for_compute_shader(1);
	m_gpu.unbind_access_view_for_compute_shader(0);
	m_gpu.unbind_compute_shader();
}

void titian::GPUTracer::display_pass()
{
	m_gpu.bind_shaders(m_display_shaders);
	m_gpu.bind_shader_view_for_pixel_shader(m_target_copy_shader_view, 0);
	m_gpu.bind_sampler_state_for_pixel_shader(m_linear_sampler, 0);

	m_gpu.draw(m_screen_mesh);

	m_gpu.unbind_sampler_state_for_pixel_shader(0);
	m_gpu.unbind_shader_view_for_pixel_shader(0);
	m_gpu.unbind_shaders();
}
