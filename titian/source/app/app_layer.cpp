#include "titian.h"


titian::AppLayer::AppLayer()
	: Layer_T("AppLayer")
{
	window.on_resize.emplace_back([this](auto size) { handle_resize(size); });
	window.set_icon("package/textures/editor_icon.ico");
	window.set_dark_mode(true);
	window.maximize();
}

titian::AppLayer::~AppLayer()
{
	cpu_dispatcher.release();
	physics.release();
	foundation.release();
}

void titian::AppLayer::set_name(const StringView& name)
{
	window.set_title(name);
}

bool titian::AppLayer::update()
{
	const TimeBomb _ = bench_time_bomb();
	timer.update();
	return window.process();
}

void titian::AppLayer::handle_resize(const Int2 size)
{
	if (size.x <= 0 || size.y <= 0) {
		return;
	}
	gpu.resize_internal(size);
	gpu.set_viewport_size(size);
}
