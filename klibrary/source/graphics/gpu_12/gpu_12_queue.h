#pragma once

#include "apis/apis.h"


namespace kl {
	class GPU12Queue
	{
	public:
		dx12::CommandQueue queue{};

		GPU12Queue();
		GPU12Queue(const dx12::CommandQueue& queue);

		void execute(const dx12::CommandList& list) const;
	};
}
