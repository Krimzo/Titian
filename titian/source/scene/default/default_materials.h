#pragma once

#include "components/material.h"


namespace titian {
	struct DefaultMaterials
	{
		Ref<Material> white = nullptr;

		DefaultMaterials(kl::GPU* gpu);
	};
}
