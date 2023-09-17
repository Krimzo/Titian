export module dll_script;

export import editor_state;

export class dll_script : public basic_script
{
public:
	kl::dll dll = {};

	kl::dll::function<void, BasicState*> start_function = nullptr;
	kl::dll::function<void, BasicState*> update_function = nullptr;

	dll_script()
	{}

	dll_script(const std::string& path)
		: basic_script(path)
	{
		dll.load(path);
		if (dll) {
			start_function = dll.read_function<void, BasicState*>("script_start");
			update_function = dll.read_function<void, BasicState*>("script_update");
		}
	}

	bool is_valid() const override
	{
		return (dll && start_function && update_function);
	}

	void reload() override
	{
		dll.reload();
	}

	void call_start(BasicState* state) override
	{
		if (dll && start_function) {
			start_function(state);
		}
	}

	void call_update(BasicState* state) override
	{
		if (dll && update_function) {
			update_function(state);
		}
	}
};
