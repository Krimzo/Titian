export module basic_script;

export import basic_state;

export class BasicScript
{
public:
	std::string path = {};

	BasicScript()
	{}

	BasicScript(const std::string_view& path)
		: path(path)
	{}

	virtual ~BasicScript()
	{}

	virtual bool is_valid() const = 0;

	virtual void reload() = 0;

	virtual void call_start(BasicState* state) = 0;

	virtual void call_update(BasicState* state) = 0;
};
