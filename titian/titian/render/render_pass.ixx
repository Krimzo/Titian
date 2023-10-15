export module render_pass;

export import state_package;
export import game_layer;

export namespace titian {
	class RenderPass
	{
	public:
		kl::Object<GameLayer> game_layer = nullptr;

		RenderPass(kl::Object<GameLayer>& game_layer)
		{
			this->game_layer = game_layer;
		}

		virtual ~RenderPass()
		{}

		void process()
		{
			if (!is_renderable()) {
				return;
			}

			StatePackage package = get_state_package();
			bind_states(package);
			render_self(package);
			unbind_states();
		}

	protected:
		virtual bool is_renderable() const = 0;
		virtual StatePackage get_state_package() = 0;
		virtual void render_self(StatePackage& package) = 0;

	private:
		void bind_states(const StatePackage& package)
		{
			const kl::GPU* gpu = &game_layer->app_layer->gpu;
			
			gpu->bind_raster_state(package.raster_state);
			gpu->bind_depth_state(package.depth_state);
			gpu->bind_render_shaders(package.shader_state);
			gpu->bind_blend_state(package.blend_state);
		}

		void unbind_states()
		{
			const kl::GPU* gpu = &game_layer->app_layer->gpu;
			
			gpu->unbind_raster_state();
			gpu->unbind_depth_state();
			gpu->unbind_render_shaders();
			gpu->unbind_blend_state();
		}
	};
}
