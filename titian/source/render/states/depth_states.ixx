export module depth_states;

export import klib;

export namespace titian {
    class DepthStates
    {
    public:
        kl::dx::DepthState disabled = nullptr;
        kl::dx::DepthState enabled = nullptr;
        
        DepthStates(kl::Object<kl::GPU>& gpu)
        {
            disabled = gpu->create_depth_state(false);
            enabled = gpu->create_depth_state(true);

            kl::assert(disabled, "Failed to inti DISABLED depth state.");
            kl::assert(enabled, "Failed to init ENABLED depth state.");
        }
    };
}
