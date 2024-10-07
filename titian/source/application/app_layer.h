#pragma once

#include "layer/layer.h"
#include "logger.h"


namespace titian {
	struct AppLayer : Layer
	{
		kl::Window window{ "Titian App" };
		kl::GPU gpu{ window };
		kl::Timer timer;

		px::PxDefaultAllocator allocator = {};
		px::PxDefaultErrorCallback error_callback = {};
		px::PxFoundation& foundation = *PxCreateFoundation(PX_PHYSICS_VERSION, allocator, error_callback);

		px::PxPhysics* physics = nullptr;
		px::PxDefaultCpuDispatcher* cpu_dispatcher = nullptr;

		AppLayer();
		~AppLayer() override;

		void init(const StringView& name);
		bool update() override;

	private:
		void handle_resize(Int2 size);
	};
}
