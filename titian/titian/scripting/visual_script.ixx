export module visual_script;

export import script;

export namespace titian {
	class VisualScript : public Script
	{
	public:
		void* scene = nullptr;

		VisualScript(void* scene)
			: Script(Type::VISUAL), scene(scene)
		{}

		~VisualScript() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Script::serialize(serializer);
		}

		void deserialize(const Serializer* serializer) override
		{
			Script::deserialize(serializer);
		}

		bool is_valid() const override
		{
			return true;
		}

		void reload() override
		{}

		void call_start() override
		{}

		void call_update() override
		{}
	};
}
