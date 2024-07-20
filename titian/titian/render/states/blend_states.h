#pragma once

#include "standard/standard.h"


namespace titian {
	class BlendStates
	{
	public:
		dx::BlendState disabled = nullptr;
		dx::BlendState enabled = nullptr;

		BlendStates(kl::GPU* gpu);
	};
}
