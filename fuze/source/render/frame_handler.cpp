#include "fuze.h"


titian::FrameHandler::FrameHandler()
{}

void titian::FrameHandler::init()
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;

	auto load_shader = [&](dx::ComputeShader& shader, str filename)
	{
		const String source = kl::read_file(kl::format("package/shaders/", filename));
		shader = gpu.create_compute_shader(source).shader;
		kl::assert(shader, "Failed to init [", filename, "] shaders");
	};

	WorkQueue queue;
	queue.add_task([&] { load_shader(m_clear_shader, "video_clear_shader.hlsl"); });
	queue.add_task([&] { load_shader(m_mix_shader, "video_mix_shader.hlsl"); });
	queue.finalize();
}

void titian::FrameHandler::prepare_frame(const Int2 size)
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;

	out_frame.resize(size);
	gpu.bind_access_view_for_compute_shader(out_frame.access_view, 0);

	const Int2 dispatch_size = (out_frame.size() / 32) + Int2(1);
	gpu.execute_compute_shader(m_clear_shader, dispatch_size.x, dispatch_size.y, 1);

	gpu.unbind_access_view_for_compute_shader(0);
}

void titian::FrameHandler::mix_frame(const Frame& frame)
{
	kl::GPU& gpu = Layers::get<AppLayer>().gpu;

	gpu.bind_access_view_for_compute_shader(out_frame.access_view, 0);
	gpu.bind_access_view_for_compute_shader(frame.access_view, 1);

	const Int2 dispatch_size = kl::min(out_frame.size(), frame.size()) / 32 + Int2(1);
	gpu.execute_compute_shader(m_mix_shader, dispatch_size.x, dispatch_size.y, 1);

	gpu.unbind_access_view_for_compute_shader(1);
	gpu.unbind_access_view_for_compute_shader(0);
}
