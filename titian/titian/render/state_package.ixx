export module state_package;

export import klib;

export namespace titian {
	class StatePackage
	{
	public:
		kl::dx::RasterState raster_state = nullptr;
		kl::dx::DepthState depth_state = nullptr;
		kl::RenderShaders shader_state = {};
		kl::dx::BlendState blend_state = nullptr;

		StatePackage()
		{}
	};
}
