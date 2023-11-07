#pragma once

#include "layer/layer.h"
#include "logger.h"


namespace titian {
	class AppLayer : public Layer
	{
	public:
		// System
		kl::Object<kl::Window> window = nullptr;
		kl::Object<kl::GPU> gpu = nullptr;
		kl::Object<kl::Timer> timer = nullptr;

		AppLayer(const std::string_view& name);

		bool update() override;
	};
}
