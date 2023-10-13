export module dll_script;

export import editor_state;

export class DLLScript : public BasicScript
{
public:
	kl::DLL dll = {};

	kl::DLL::Function<void, BasicState*> start_function = nullptr;
	kl::DLL::Function<void, BasicState*> update_function = nullptr;

	DLLScript()
	{}

	DLLScript(const std::string& path)
		: BasicScript(path)
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
