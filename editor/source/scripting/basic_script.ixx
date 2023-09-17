export module basic_script;

export import basic_state;

export class basic_script
{
public:
	std::string path = {};

	basic_script()
	{}

	basic_script(const std::string& path)
		: path(path)
	{}

	virtual ~basic_script()
	{}

	virtual bool is_valid() const = 0;
	virtual void reload() = 0;

	virtual void call_start(BasicState* state) = 0;
	virtual void call_update(BasicState* state) = 0;
};
