export module dll_script;

export import script;

export namespace titian {
	class DLLScript : public Script
	{
	public:
		void* scene = nullptr;

		std::string path = {};
		kl::DLL dll = {};

		kl::DLL::Function<void, void*> start_function = nullptr;
		kl::DLL::Function<void, void*> update_function = nullptr;

		DLLScript(void* scene)
			: Script(Type::DLL), scene(scene)
		{}

		DLLScript(void* scene, const std::string& path)
			: Script(Type::DLL), scene(scene)
		{
			dll.load(path);
			this->reload();
		}

		~DLLScript() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Script::serialize(serializer);

			serializer->write_string(path);
		}

		void deserialize(const Serializer* serializer) override
		{
			Script::deserialize(serializer);

			serializer->read_string(path);
			this->reload();
		}

		bool is_valid() const override
		{
			return dll && start_function && update_function;
		}

		void reload() override
		{
			dll.reload();
			if (dll) {
				start_function = dll.read_function<void, void*>("script_start");
				update_function = dll.read_function<void, void*>("script_update");
			}
		}

		void call_start() override
		{
			if (start_function) {
				start_function(scene);
			}
		}

		void call_update() override
		{
			if (update_function) {
				update_function(scene);
			}
		}
	};
}
