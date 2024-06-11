#include "main.h"


titian::AppLayer::AppLayer()
	: Layer("AppLayer")
{}

void titian::AppLayer::init(const std::string_view& name)
{
	ImGui_ImplWin32_EnableDpiAwareness();
	window.set_title(name);
	window.on_resize.emplace_back([this](auto size) { handle_resize(size); });
	window.set_icon("builtin/textures/editor_icon.ico");
	window.set_dark_mode(true);
	window.maximize();
}

bool titian::AppLayer::update()
{
	const TimeBomb _ = this->time_it();

	timer.update_delta();
	return window.process(false);
}

void titian::AppLayer::handle_resize(const kl::Int2 size)
{
	if (size.x <= 0 || size.y <= 0) {
		return;
	}
	gpu.resize_internal(size);
	gpu.set_viewport_size(size);
}
