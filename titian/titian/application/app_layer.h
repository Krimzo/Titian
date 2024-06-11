#pragma once

#include "layer/layer.h"
#include "logger.h"


namespace titian {
	class AppLayer : public Layer
	{
	public:
		// System
		kl::Window window{ "TITIAN", {1600, 900} };
		kl::GPU gpu{ static_cast<HWND>(window), kl::IS_DEBUG, true };
		kl::Timer timer{};

		AppLayer();

		void init(const std::string_view& name);
		bool update() override;

	private:
		void handle_resize(kl::Int2 size);
	};
}
