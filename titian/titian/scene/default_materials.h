#pragma once

#include "components/material.h"


namespace titian {
	class DefaultMaterials
	{
	public:
		kl::Object<Material> white = nullptr;

		DefaultMaterials(kl::GPU* gpu);
	};
}
