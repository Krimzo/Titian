export module native_script;

export import script;
export import memorymodule;

export namespace titian {
	class NativeScript : public Script
	{
	public:
		template<typename Return, typename... Args>
		using Function = Return(__stdcall*)(Args...);

		std::vector<byte> data = {};

		NativeScript(void* scene)
			: Script(Type::NATIVE, scene)
		{}

		~NativeScript() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Script::serialize(serializer);

			const uint64_t size = data.size();
			serializer->write_object<uint64_t>(size);
			serializer->write_array<byte>(data.data(), size);
		}

		void deserialize(const Serializer* serializer) override
		{
			Script::deserialize(serializer);

			const uint64_t size = serializer->read_object<uint64_t>();
			data.resize(size);
			serializer->read_array(data.data(), size);
			this->reload();
		}

		bool is_valid() const override
		{
			return m_memory_module && m_start_function && m_update_function;
		}

		void reload() override
		{
			this->unload();
			m_memory_module = MemoryLoadLibrary(data.data(), data.size());
			m_start_function = read_function<void, void*>("script_start");
			m_update_function = read_function<void, void*>("script_update");
		}

		void call_start() override
		{
			if (m_start_function) {
				m_start_function(scene);
			}
		}

		void call_update() override
		{
			if (m_update_function) {
				m_update_function(scene);
			}
		}

	private:
		void* m_memory_module = nullptr;
		Function<void, void*> m_start_function = nullptr;
		Function<void, void*> m_update_function = nullptr;

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
