export module render_layer;

export import application_layer;
export import raster_states;
export import depth_states;
export import sampler_states;
export import shader_states;

export namespace titian {
	class RenderLayer : public Layer
	{
	public:
		kl::Object<ApplicationLayer> app_layer = nullptr;

		kl::Object<RasterStates> raster_states = nullptr;
		kl::Object<DepthStates> depth_states = nullptr;
		kl::Object<SamplerStates> sampler_states = nullptr;
		kl::Object<ShaderStates> shader_states = nullptr;

		RenderLayer(kl::Object<ApplicationLayer>& app_layer)
		{
			this->app_layer = app_layer;

			raster_states = new RasterStates(app_layer->gpu);
			depth_states = new DepthStates(app_layer->gpu);
			sampler_states = new SamplerStates(app_layer->gpu);
			shader_states = new ShaderStates(app_layer->gpu);
		}

		~RenderLayer() override
		{}

		bool update() override
		{
			kl::GPU* gpu = &app_layer->gpu;

			gpu->clear_internal(kl::Color(30, 30, 30));
			return true;
		}
	};
}
