export module application_layer;

export import layer;
export import log_info;

export namespace titian {
	class ApplicationLayer : public Layer
	{
	public:
		// System
		kl::Object<kl::Window> window = nullptr;
		kl::Object<kl::GPU> gpu = nullptr;
		kl::Object<kl::Timer> timer = nullptr;

		// Logging
		std::vector<LogInfo> logs = {};

		ApplicationLayer(const std::string_view& name)
		{
			window = new kl::Window(name.data(), {1920, 1080});
			gpu = new kl::GPU(*window, kl::IS_DEBUG, true);
			timer = new kl::Timer();

			window->on_resize.emplace_back([&](const kl::Int2 new_size)
			{
				gpu->resize_internal(new_size);
				gpu->set_viewport_size(new_size);
			});
			window->maximize();
		}

		~ApplicationLayer() override
		{}

		bool update() override
		{
			timer->update_delta();
			return window->process(false);
		}

		void log(const LogInfo& info)
		{
			logs.push_back(info);
		}
	};
}
