export module script;

export import serializable;

export namespace titian {
	class Script : public Serializable
	{
	public:
		enum class Type
		{
			DLL,
			VISUAL,
		} const type = {};

		void* scene = nullptr;

		Script(const Type type, void* scene)
			: type(type), scene(scene)
		{}

		~Script() override
		{}

		void serialize(Serializer* serializer) const override
		{
			serializer->write_object<Type>(type);
		}

		void deserialize(const Serializer* serializer) override
		{}

		virtual bool is_valid() const = 0;
		virtual void reload() = 0;

		virtual void call_start() = 0;
		virtual void call_update() = 0;
	};
}
