#pragma once

#include "apis/apis.h"


namespace kl {
	struct GPU12Queue
	{
		dx12::CommandQueue queue{};

		GPU12Queue();
		GPU12Queue(const dx12::CommandQueue& queue);

		void execute(const dx12::CommandList& list) const;
	};
}
