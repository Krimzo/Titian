export module jvm_script;

export import editor_state;

export class JVMScript : public BasicScript
{
public:
	JavaHolder<jobject> loader { nullptr };
	JavaHolder<jclass> clazz { nullptr };
	JavaHolder<jobject> instance { nullptr };

	jmethod start_method = {};
	jmethod update_method = {};

	JVMScript()
	{}

	JVMScript(const std::string& path)
		: BasicScript(path)
	{
		this->reload();
	}

	bool is_valid() const override
	{
		return (instance && start_method && update_method);
	}

	void reload() override
	{
		// Reset
		update_method = nullptr;
		start_method = nullptr;
		instance.set(nullptr);
		loader.set(nullptr);
		clazz.set(nullptr);

		// Load
		loader.set(java::new_loader());
		clazz.set(java::load_class(loader.get(), java::read_class_data(path)));

		jmethod constructor = java::get_constructor(clazz.get());
		if (!constructor) {
			return;
		}
		jobject object = java::new_object(clazz.get(), constructor);
		if (!object) {
			return;
		}

		instance.set(object);
		start_method = java::get_method(clazz.get(), "start");
		update_method = java::get_method(clazz.get(), "update");
	}

	void call_start(BasicState* state) override
	{
		if (instance && start_method) {
			java::jni->CallVoidMethod(instance.get(), start_method);
		}
	}

	void call_update(BasicState* state) override
	{
		if (instance || start_method) {
			java::jni->CallVoidMethod(instance.get(), update_method);
		}
	}
};