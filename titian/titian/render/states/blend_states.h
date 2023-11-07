#pragma once

#include "klibrary.h"


namespace titian {
	class BlendStates
	{
	public:
		kl::dx::BlendState disabled = nullptr;
		kl::dx::BlendState enabled = nullptr;

		BlendStates(kl::GPU* gpu);
	};
}
