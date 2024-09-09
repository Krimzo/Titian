#pragma once

#include "standard.h"


namespace titian {
	struct BlendStates
	{
		dx::BlendState disabled;
		dx::BlendState enabled;

		BlendStates(kl::GPU* gpu);
	};
}
