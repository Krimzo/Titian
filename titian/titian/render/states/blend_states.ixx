export module blend_states;

export import klib;

export namespace titian {
	class BlendStates
	{
	public:
		kl::dx::BlendState disabled = nullptr;
		kl::dx::BlendState enabled = nullptr;

		BlendStates(kl::GPU* gpu)
		{
			disabled = gpu->create_blend_state(false);
			enabled = gpu->create_blend_state(true);

			kl::assert(disabled, "Failed to inti DISABLED blend state.");
			kl::assert(enabled, "Failed to init ENABLED blend state.");
		}
	};
}
