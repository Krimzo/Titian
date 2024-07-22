#pragma once

#include "layer/layer.h"
#include "logger.h"


namespace titian {
	class AppLayer : public Layer
	{
	public:
		kl::Window window{ "Titian App", {1600, 900} };
		kl::GPU gpu{ static_cast<HWND>(window), kl::IS_DEBUG, true };
		kl::Timer timer{};

		AppLayer();

		void init(const String& name);
		bool update() override;

	private:
		void handle_resize(Int2 size);
	};
}
