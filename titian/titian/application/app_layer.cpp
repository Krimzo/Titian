#include "main.h"


titian::AppLayer::AppLayer(const std::string_view& name)
	: Layer("AppLayer")
{
	window = new kl::Window(name.data(), { 1600, 900 });
	gpu = new kl::GPU(*window, kl::IS_DEBUG, true);
	timer = new kl::Timer();

	window->on_resize.emplace_back([&](const kl::Int2 new_size)
	{
		if (new_size.x > 0 && new_size.y > 0) {
			gpu->resize_internal(new_size);
			gpu->set_viewport_size(new_size);
		}
	});

	{
		BOOL dark_mode = true;
		DwmSetWindowAttribute(
				*window, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
				&dark_mode, sizeof(dark_mode));
	}

	window->set_icon("builtin/textures/editor_icon.ico");
	window->maximize();
}

bool titian::AppLayer::update()
{
	const TimeBomb _ = this->time_it();

	timer->update_delta();
	return window->process(false);
}
