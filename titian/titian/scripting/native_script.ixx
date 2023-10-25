export module native_script;

export import game_layer;
export import memorymodule;

export namespace titian {
	class NativeScript : public Script
	{
	public:
		template<typename Return, typename... Args>
		using Function = Return(__stdcall*)(Args...);

		std::vector<byte> data = {};
		std::string path = {};

		NativeScript()
			: Script(Type::NATIVE)
		{}

		~NativeScript() override
		{}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			Script::serialize(serializer, helper_data);

			const uint64_t size = data.size();
			serializer->write_object<uint64_t>(size);
			serializer->write_array<byte>(data.data(), size);
			serializer->write_string(path);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			Script::deserialize(serializer, helper_data);

			const uint64_t size = serializer->read_object<uint64_t>();
			data.resize(size);
			serializer->read_array(data.data(), size);
			serializer->read_string(path);
			this->reload();
		}

		bool is_valid() const override
		{
			return m_memory_module && m_start_function && m_update_function;
		}

		void reload() override
		{
			this->unload();

			if (!path.empty()) {
				data = kl::read_file(path);
			}

			m_memory_module = MemoryLoadLibrary(data.data(), data.size());
			m_start_function = read_function<void, Scene*>("on_start");
			m_update_function = read_function<void, Scene*>("on_update");
		}

		void call_start() override
		{
			Scene* scene = &GameLayer::BOUND_SELF->scene;
			if (m_start_function && scene) {
				m_start_function(scene);
			}
		}

		void call_update() override
		{
			Scene* scene = &GameLayer::BOUND_SELF->scene;
			if (m_update_function && scene) {
				m_update_function(scene);
			}
		}

	private:
		void* m_memory_module = nullptr;
		Function<void, Scene*> m_start_function = nullptr;
		Function<void, Scene*> m_update_function = nullptr;

		void unload()
		{
			m_start_function = nullptr;
			m_update_function = nullptr;
			if (m_memory_module) {
				MemoryFreeLibrary(m_memory_module);
				m_memory_module = nullptr;
			}
		}

		template<typename Return, typename... Args>
		Function<Return, Args...> read_function(const std::string_view& function_name)
		{
			if (m_memory_module) {
				auto function_address = MemoryGetProcAddress(m_memory_module, function_name.data());
				return reinterpret_cast<Function<Return, Args...>>(function_address);
			}
			return nullptr;
		}
	};
}
