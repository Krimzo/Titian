#pragma once

#include "layer/layer.h"
#include "logger.h"


namespace titian {
	struct AppLayer : Layer
	{
		kl::Window window{ "Titian App", {1600, 900} };
		kl::GPU gpu{ HWND(window), kl::IS_DEBUG, true, false };
		kl::Timer timer{};

		AppLayer();

		void init(const StringView& name);
		bool update() override;

	private:
		void handle_resize(Int2 size);
	};
}
