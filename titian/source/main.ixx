export module main;

export import titian_entry;
export import sandbox_entry;

export int main(const int argc, const char** argv)
{
	int code = sandbox_entry();
	return code;
}
