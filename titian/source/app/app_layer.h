#pragma once

#include "layer/layer.h"
#include "logger.h"


namespace titian {
	struct AppLayer : Layer_T<AppLayer>
	{
		kl::Window window{ "Titian App" };
		kl::GPU gpu{ window };
		kl::Timer timer;

		px::PxDefaultAllocator allocator = {};
		px::PxDefaultErrorCallback error_callback = {};
		px::PxFoundation& foundation = *PxCreateFoundation(PX_PHYSICS_VERSION, allocator, error_callback);
		px::PxPhysics& physics = *PxCreatePhysics(PX_PHYSICS_VERSION, foundation, px::PxTolerancesScale{});
		px::PxDefaultCpuDispatcher& cpu_dispatcher = *px::PxDefaultCpuDispatcherCreate(2);

		AppLayer();
		~AppLayer() override;

		void set_name(const StringView& name);
		bool update() override;

	private:
		void handle_resize(Int2 size);
	};
}
