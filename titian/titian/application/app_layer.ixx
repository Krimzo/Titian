export module app_layer;

export import layer;
export import logger;

export namespace titian {
	class AppLayer : public Layer
	{
	public:
		// System
		kl::Object<kl::Window> window = nullptr;
		kl::Object<kl::GPU> gpu = nullptr;
		kl::Object<kl::Timer> timer = nullptr;

		AppLayer(const std::string_view& name)
		{
			window = new kl::Window(name.data(), {1920, 1080});
			gpu = new kl::GPU(*window, kl::IS_DEBUG, true);
			timer = new kl::Timer();

			window->on_resize.emplace_back([&](const kl::Int2 new_size)
			{
				if (new_size.x > 0 && new_size.y > 0) {
					gpu->resize_internal(new_size);
					gpu->set_viewport_size(new_size);
				}
			});
			window->set_icon("builtin/textures/editor_icon.ico");
			window->maximize();

			if (!kl::IS_DEBUG) {
				kl::console::set_enabled(false);
			}
		}

		~AppLayer() override
		{
			kl::console::set_enabled(true);
		}

		bool update() override
		{
			timer->update_delta();
			return window->process(false);
		}
	};
}
