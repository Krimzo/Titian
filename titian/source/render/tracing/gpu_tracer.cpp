#include "titian.h"


titian::GPUTracer::GPUTracer(kl::Window& window, const TracingScene& scene, const Int2 resolution)
	: window(window), scene(scene), m_gpu(window.ptr())
{
	//m_target.resize(resolution);
}

void titian::GPUTracer::render()
{
	while (window.process()) {
		handle_input();
		m_gpu.clear_internal();
		m_gpu.swap_buffers(true);
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
			//m_target.save_to_file(path.value(), kl::ImageType::PNG);
		}
	}
}
