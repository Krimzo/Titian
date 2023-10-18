export module layer;

export import klib;

export namespace titian {
	class Layer
	{
	public:
		Layer()
		{}

		virtual ~Layer()
		{}

		virtual bool update() = 0;

	protected:
		void panic() const
		{
			const std::string message = kl::format("Layer 0x", std::hex, (size_t) this, " panicked");
			kl::assert(false, message, false);
		}
	};
}
