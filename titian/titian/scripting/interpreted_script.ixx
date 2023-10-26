export module interpreted_script;

export import interpreted_script_parts;

export namespace titian {
	class InterpretedScript : public Script
	{
	public:
		std::string source = {};
		std::string path = {};

		InterpretedScript()
			: Script(Type::INTERPRETED)
		{
			this->source = "def on_start(scene) {\n\t\n}\n\ndef on_update(scene) {\n\t\n}\n";
		}

		~InterpretedScript() override
		{}

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			Script::serialize(serializer, helper_data);

			serializer->write_string(source);
			serializer->write_string(path);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{
			Script::deserialize(serializer, helper_data);

			serializer->read_string(source);
			serializer->read_string(path);
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

			if (std::filesystem::exists(path)) {
				source = kl::read_file_string(path);
			}

			try {
				m_engine->add(INTERPRETED_SCRIPT_MODULE);
				m_engine->eval(this->source);
				m_start_function = m_engine->eval<std::function<void(Scene*)>>("on_start");
				m_update_function = m_engine->eval<std::function<void(Scene*)>>("on_update");
			}
			catch (std::exception& e) {
				Logger::log(e.what());
			}
		}

		void call_start() override
		{
			Scene* scene = &GameLayer::BOUND_SELF->scene;
			if (m_start_function && scene) {
				try {
					m_start_function(scene);
				}
				catch (std::exception& e) {
					Logger::log(e.what());
				}
			}
		}

		void call_update() override
		{
			Scene* scene = &GameLayer::BOUND_SELF->scene;
			if (m_update_function && scene) {
				try {
					m_update_function(scene);
				}
				catch (std::exception& e) {
					Logger::log(e.what());
				}
			}
		}

	private:
		kl::Object<chaiscript::ChaiScript> m_engine = nullptr;
		std::function<void(Scene*)> m_start_function = {};
		std::function<void(Scene*)> m_update_function = {};
	};
}
