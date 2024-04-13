#include "main.h"


titian::AppLayer::AppLayer(const std::string_view& name)
	: Layer("AppLayer")
{
	ImGui_ImplWin32_EnableDpiAwareness();

	window = new kl::Window(name.data(), { 1600, 900 });
	gpu = new kl::GPU(*window, kl::IS_DEBUG, true);
	timer = new kl::Timer();

	window->on_resize.emplace_back([this](auto size) { handle_resize(size); });
	window->set_icon("builtin/textures/editor_icon.ico");
	window->set_dark_mode(true);
	window->maximize();
}

bool titian::AppLayer::update()
{
	const TimeBomb _ = this->time_it();

	timer->update_delta();
	return window->process(false);
}

void titian::AppLayer::handle_resize(const kl::Int2 size)
{
	if (size.x <= 0 || size.y <= 0) {
		return;
	}
	gpu->resize_internal(size);
	gpu->set_viewport_size(size);
}
