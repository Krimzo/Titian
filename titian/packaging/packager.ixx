export module packager;

export import serializer;

export namespace titian {
	const std::string PACKAGE_EXTENSION = ".tipk";

	const std::string BUILTIN_DIR = "builtin";
	const std::string PREVIEW_DIR = "preview";

	const std::string PACKAGED_BUILTIN_FILE = BUILTIN_DIR + PACKAGE_EXTENSION;
	const std::string PACKAGED_PREVIEW_FILE = PREVIEW_DIR + PACKAGE_EXTENSION;
}

export namespace titian {
	std::set<std::string> list_files(const std::string_view& dir)
	{
		std::set<std::string> found_files = {};
		for (const auto& path : std::filesystem::recursive_directory_iterator(dir)) {
			if (!path.is_directory()) {
				found_files.insert(path.path().string());
			}
		}
		return found_files;
	}

	bool create_package(const std::string_view& input_dir, const std::string& output_file)
	{
		if (!std::filesystem::exists(input_dir)) {
			Logger::log("Create package error. ", input_dir, " doesn't exists.");
			return false;
		}
		
		Serializer serializer = { output_file, true };
		if (!serializer) {
			Logger::log("Create package error. Failed to open file ", output_file);
			return false;
		}
		
		const std::set<std::string> files = list_files(input_dir);
		serializer.write_object<uint64_t>(files.size());
		for (const auto& file : files) {
			const std::vector<byte> file_data = kl::read_file(file);
			serializer.write_string(file);
			serializer.write_object<uint64_t>(file_data.size());
			serializer.write_array(file_data.data(), file_data.size());
			Logger::log("Saved file ", file, " to package ", output_file);
		}

		Logger::log("Created packaged ", output_file, " from ", input_dir);
		return true;
	}

	bool open_package(const std::string& input_file)
	{
		const Serializer serializer = { input_file, false };
		if (!serializer) {
			Logger::log("Open package error. Failed to open file ", input_file);
			return false;
		}

		const uint64_t file_count = serializer.read_object<uint64_t>();
		for (uint64_t i = 0; i < file_count; i++) {
			const std::string file = serializer.read_string();
			const uint64_t file_size = serializer.read_object<uint64_t>();

			std::vector<byte> file_data = {};
			file_data.resize(file_size);
			serializer.read_array(file_data.data(), file_size);

			const std::filesystem::path path(file);
			if (path.has_parent_path()) {
				const std::filesystem::path parent_dir = path.parent_path();
				if (std::filesystem::create_directories(parent_dir)) {
					Logger::log("Created parent directory ", parent_dir);
				}

				kl::write_file(file, file_data);
				Logger::log("Loaded file ", file, " from package ", input_file);
			}
		}
		return true;
	}
}
