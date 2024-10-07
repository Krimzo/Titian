#include "titian.h"


titian::AppLayer::AppLayer()
	: Layer("AppLayer")
{}

titian::AppLayer::~AppLayer()
{
	cpu_dispatcher->release();
	physics->release();
	foundation.release();
}

void titian::AppLayer::init(const StringView& name)
{
	window.set_title(name);
	window.on_resize.emplace_back([this](auto size) { handle_resize(size); });
	window.set_icon("package/textures/editor_icon.ico");
	window.set_dark_mode(true);
	window.maximize();

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, foundation, px::PxTolerancesScale{});
	cpu_dispatcher = px::PxDefaultCpuDispatcherCreate(2);
	kl::assert(physics, "Failed to create physics");
	kl::assert(cpu_dispatcher, "Failed to create physics dispatcher");
}

bool titian::AppLayer::update()
{
	const TimeBomb _ = bench_time_bomb();

	timer.update_delta();
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
