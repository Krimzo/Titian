#pragma once

#include "components/material.h"


namespace titian {
	class DefaultMaterials
	{
	public:
		Ref<Material> white = nullptr;

		DefaultMaterials(kl::GPU* gpu);
	};
}
