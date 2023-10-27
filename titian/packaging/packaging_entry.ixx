export module packaging_entry;

export import packager;

export int packaging_entry(const int create)
{
	using namespace titian;

	bool result = false;
	if (create) {
		result = create_package(BUILTIN_DIR, PACKAGED_BUILTIN_FILE);
		result = create_package(PREVIEW_DIR, PACKAGED_PREVIEW_FILE);
	}
	else {
		result = open_package(PACKAGED_BUILTIN_FILE);
		result = open_package(PACKAGED_PREVIEW_FILE);
	}
	return static_cast<int>(result);
}
