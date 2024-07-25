#pragma once

#include "standard.h"


namespace titian {
	class BlendStates
	{
	public:
		dx::BlendState disabled;
		dx::BlendState enabled;

		BlendStates(kl::GPU* gpu);
	};
}
