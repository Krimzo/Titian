#pragma once

#include "apis/apis.h"


namespace kl {
	struct GPU12Fence
	{
		dx12::Fence fence{};
		uint64_t counter{};

		GPU12Fence();
		GPU12Fence(const dx12::Fence& fence);

		void signal_and_wait(const dx12::CommandQueue& queue);
	};
}
