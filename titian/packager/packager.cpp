#include "main.h"


titian::Set<titian::String> packager::list_files(const String& input)
{
	if (!fs::is_directory(input)) {
		return { input };
	}

	Set<String> found_files;
	for (const auto& entry : fs::recursive_directory_iterator(input)) {
		if (!entry.is_directory()) {
			found_files.insert(entry.path().relative_path().string());
		}
	}
	return found_files;
}

bool packager::create_package(const String& input, const String& output_file)
{
	if (!fs::exists(input)) {
		Logger::log("Create package error. ", input, " doesn't exists.");
		return false;
	}

	Serializer serializer = { output_file, true };
	if (!serializer) {
		Logger::log("Create package error. Failed to open file ", output_file);
		return false;
	}

	const Set<String> files = list_files(input);
	serializer.write_object<uint64_t>(files.size());
	for (const auto& file : files) {
		const Vector<byte> file_data = kl::read_file(file);
		serializer.write_string(file);
		serializer.write_object<uint64_t>(file_data.size());
		serializer.write_array(file_data.data(), file_data.size());
		Logger::log("Saved file ", file, " to package ", output_file);
	}

	Logger::log("Created packaged ", output_file, " from ", input);
	return true;
}

bool packager::open_package(const String& input_file, const String& output_dir)
{
	const Serializer serializer = { input_file, false };
	if (!serializer) {
		Logger::log("Open package error. Failed to open file ", input_file);
		return false;
	}

	const uint64_t file_count = serializer.read_object<uint64_t>();
	for (uint64_t i = 0; i < file_count; i++) {
		const String file = output_dir + "/" + serializer.read_string();
		const uint64_t file_size = serializer.read_object<uint64_t>();

		Vector<byte> file_data;
		file_data.resize(file_size);
		serializer.read_array(file_data.data(), file_size);

		const fs::path path(file);
		if (path.has_parent_path()) {
			const fs::path parent_dir = path.parent_path();
			if (fs::create_directories(parent_dir)) {
				Logger::log("Created parent directory ", parent_dir.string());
			}

			kl::write_file(file, file_data);
			Logger::log("Loaded file ", file, " from package ", input_file);
		}
	}
	return true;
}
