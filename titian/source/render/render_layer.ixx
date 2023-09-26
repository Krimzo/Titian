export module render_layer;

export import application_layer;
export import raster_states;
export import depth_states;
export import sampler_states;
export import shader_states;

export namespace titian {
	class RenderLayer : public Layer
	{
		kl::Object<ApplicationLayer> m_app_layer = nullptr;
		kl::GPU m_gpu = {};
		
		RasterStates m_raster_states = { &m_gpu };
		DepthStates m_depth_states = { &m_gpu };
		SamplerStates m_sampler_states = { &m_gpu };
		ShaderStates m_shader_states = { &m_gpu };

	public:
		RenderLayer(const kl::Object<ApplicationLayer>& app_layer)
			: m_app_layer(app_layer), m_gpu((HWND) app_layer->window(), kl::IS_DEBUG)
		{
			m_app_layer->window().on_resize.emplace_back([&](const kl::Int2 new_size)
			{
				m_gpu.resize_internal(new_size);
				m_gpu.set_viewport_size(new_size);
			});
			m_app_layer->window().maximize();
		}

		~RenderLayer() override
		{}

		bool update() override
		{
			m_gpu.clear_internal(kl::Color(30, 30, 30));
			m_gpu.swap_buffers(true);
			return true;
		}

		kl::dx::Device device() const
		{
			return m_gpu.device();
		}

		kl::dx::Context context() const
		{
			return m_gpu.context();
		}
	};
}
