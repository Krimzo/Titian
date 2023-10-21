export module interpreted_script;

export import script;
export import interpreted_script_parts;

export namespace titian {
	class InterpretedScript : public Script
	{
	public:
		std::string source = {};

		InterpretedScript(void* scene)
			: Script(Type::INTERPRETED, scene)
		{}

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
			return m_start_function && m_update_function;
		}

		void reload() override
		{}

		void call_start() override
		{
			if (m_start_function) {
				m_start_function();
			}
		}

		void call_update() override
		{
			if (m_update_function) {
				m_update_function();
			}
		}

	private:
		chaiscript::ChaiScript m_engine = {};
		std::function<void()> m_start_function = {};
		std::function<void()> m_update_function = {};
	};
}
