export module application_layer;

export import layer;

export namespace titian {
	class ApplicationLayer : public Layer
	{
		kl::Window m_window;
		kl::Timer m_timer = {};

	public:
		ApplicationLayer(const std::string_view& name)
			: m_window(name.data(), { 1600, 900 })
		{}

		~ApplicationLayer() override
		{}

		bool update() override
		{
			m_timer.update_delta();
			return m_window.process(false);
		}

		kl::Window& window()
		{
			return m_window;
		}

		const kl::Window& window() const
		{
			return m_window;
		}

		float elapsed_time() const
		{
			return m_timer.elapsed();
		}

		float delta_time() const
		{
			return m_timer.delta();
		}
	};
}
