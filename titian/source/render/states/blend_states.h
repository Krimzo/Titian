#pragma once

#include "standard.h"


namespace titian {
	using namespace standard;
}

namespace titian {
	class BlendStates
	{
	public:
		dx::BlendState disabled = nullptr;
		dx::BlendState enabled = nullptr;

		BlendStates(kl::GPU* gpu);
	};
}