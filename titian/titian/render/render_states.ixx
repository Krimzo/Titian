export module render_states;

export import raster_states;
export import depth_states;
export import sampler_states;
export import shader_states;
export import blend_states;

export namespace titian {
	class RenderStates
	{
	public:
		kl::Object<RasterStates> raster_states = nullptr;
		kl::Object<DepthStates> depth_states = nullptr;
		kl::Object<SamplerStates> sampler_states = nullptr;
		kl::Object<ShaderStates> shader_states = nullptr;
		kl::Object<BlendStates> blend_states = nullptr;

		RenderStates(kl::GPU* gpu)
		{
			raster_states = new RasterStates(gpu);
			depth_states = new DepthStates(gpu);
			sampler_states = new SamplerStates(gpu);
			shader_states = new ShaderStates(gpu);
			blend_states = new BlendStates(gpu);
		}
	};
}
