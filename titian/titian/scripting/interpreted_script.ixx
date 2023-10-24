export module interpreted_script;

export import script;
export import interpreted_script_parts;
export import logger;

export namespace titian {
	class InterpretedScript : public Script
	{
	public:
		std::string source = {};

		InterpretedScript(void* scene)
			: Script(Type::INTERPRETED, scene)
		{
			this->source = "def on_start {\n\t\n}\n\ndef on_update {\n\t\n}\n";
		}

		~InterpretedScript() override
		{}

		void serialize(Serializer* serializer) const override
		{
			Script::serialize(serializer);

			serializer->write_string(source);
		}

		void deserialize(const Serializer* serializer) override
		{
			Script::deserialize(serializer);

			serializer->read_string(source);
			this->reload();
		}

		bool is_valid() const override
		{
			return m_engine && m_start_function && m_update_function;
		}

		void reload() override
		{
			m_start_function = {};
			m_update_function = {};
			m_engine = new chaiscript::ChaiScript();

			try {
				m_engine->add(INTERPRETED_SCRIPT_MODULE);
				m_engine->eval(INTERPRETED_SCRIPT_PREDEFINED_SOURCE);
				m_engine->eval(this->source);
				m_start_function = m_engine->eval<std::function<void()>>("on_start");
				m_update_function = m_engine->eval<std::function<void()>>("on_update");
			}
			catch (std::exception& e) {
				Logger::log(e.what());
			}
		}

		void call_start() override
		{
			if (m_start_function) {
				try {
					m_start_function();
				}
				catch (std::exception& e) {
					Logger::log(e.what());
				}
			}
		}

		void call_update() override
		{
			if (m_update_function) {
				try {
					m_update_function();
				}
				catch (std::exception& e) {
					Logger::log(e.what());
				}
			}
		}

	private:
		kl::Object<chaiscript::ChaiScript> m_engine = nullptr;
		std::function<void()> m_start_function = {};
		std::function<void()> m_update_function = {};
	};
}
