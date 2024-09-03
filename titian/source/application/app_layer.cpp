#include "titian.h"


titian::AppLayer::AppLayer()
	: Layer("AppLayer")
{}

void titian::AppLayer::init(const StringView& name)
{
	ImGui_ImplWin32_EnableDpiAwareness();
	window.set_title(name);
	window.on_resize.emplace_back([this](auto size) { handle_resize(size); });
	window.set_icon("package/textures/editor_icon.ico");
	window.set_dark_mode(true);
	window.maximize();
}

bool titian::AppLayer::update()
{
	const TimeBomb _ = bench_time_bomb();

	timer.update_delta();
	return window.process(false);
}

void titian::AppLayer::handle_resize(const Int2 size)
{
	if (size.x <= 0 || size.y <= 0) {
		return;
	}
	gpu.resize_internal(size);
	gpu.set_viewport_size(size);
}
