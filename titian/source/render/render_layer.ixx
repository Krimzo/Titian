export module render_layer;

export import application_layer;
export import render_states;
export import render_pass;

export namespace titian {
	class RenderLayer : public Layer
	{
	public:
		kl::Object<ApplicationLayer> app_layer = nullptr;

		kl::Object<RenderStates> states = nullptr;
		std::vector<kl::Object<RenderPass>> passes = {};

		kl::Float4 background = kl::Color(30, 30, 30);
		bool render_wireframe = false;
		bool render_colliders = true;
		bool v_sync = true;

		RenderLayer(kl::Object<ApplicationLayer>& app_layer)
		{
			this->app_layer = app_layer;
			states = new RenderStates(app_layer->gpu);
		}

		~RenderLayer() override
		{}

		bool update() override
		{
			kl::GPU* gpu = &app_layer->gpu;

			gpu->clear_internal(background);
			for (auto& pass : passes) {
				pass->process();
			}
			return true;
		}

		void present() const
		{
			app_layer->gpu->swap_buffers(v_sync);
		}
	};
}
