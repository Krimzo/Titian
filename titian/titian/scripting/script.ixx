export module script;

export import serializable;

export namespace titian {
	class Script : public Serializable
	{
	public:
		enum class Type
		{
			NATIVE,
			INTERPRETED,
			NODE,
		} const type = {};

		Script(const Type type)
			: type(type)
		{}

		~Script() override
		{}

		Script(const Script&) = delete;
		Script(const Script&&) = delete;

		void operator=(const Script&) = delete;
		void operator=(const Script&&) = delete;

		void serialize(Serializer* serializer, const void* helper_data) const override
		{
			serializer->write_object<Type>(type);
		}

		void deserialize(const Serializer* serializer, const void* helper_data) override
		{}

		virtual bool is_valid() const = 0;
		virtual void reload() = 0;

		virtual void call_start() = 0;
		virtual void call_update() = 0;
	};
}
