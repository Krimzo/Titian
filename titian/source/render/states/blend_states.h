#pragma once

#include "standard.h"


namespace titian {
	struct BlendStates
	{
		dx::BlendState disabled;
		dx::BlendState enabled;

		void init(kl::GPU& gpu);
	};
}
