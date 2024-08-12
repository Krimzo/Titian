#include "packager.h"


int titian::packager_entry(const int argc, const char** argv, const bool package)
{
	bool result = false;
	if (package) {
		if (argc >= 4) {
			result = create_package(argv[2], argv[3]);
		}
		else {
			Logger::log("Usage: ... <input> <output_file>");
		}
	}
	else {
		if (argc >= 4) {
			result = open_package(argv[2], argv[3]);
		}
		else {
			Logger::log("Usage: ... <input_file> <output_dir>");
		}
	}
	return result ? 0 : 1;
}

titian::Set<titian::String> titian::list_files(const String& input)
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

bool titian::create_package(const String& input, const String& output_file)
{
	if (!fs::exists(input)) {
		Logger::log("Create package error. ", input, " doesn't exists.");
		return false;
	}

	BinarySerializer serializer{ output_file, true };
	if (!serializer) {
		Logger::log("Create package error. Failed to open file ", output_file);
		return false;
	}

	const Set<String> files = list_files(input);
	serializer.write_int("files_size", (int32_t) files.size());
	for (const auto& file : files) {
		const String file_data = kl::read_file(file);
		serializer.write_string("file", file);
		serializer.write_int("file_data_size", (int32_t) file_data.size());
		serializer.write_byte_array("file_data", file_data.data(), (int32_t) file_data.size());
		Logger::log("Saved file ", file, " to package ", output_file);
	}

	Logger::log("Created packaged ", output_file, " from ", input);
	return true;
}

bool titian::open_package(const String& input_file, const String& output_dir)
{
	const BinarySerializer serializer{ input_file, false };
	if (!serializer) {
		Logger::log("Open package error. Failed to open file ", input_file);
		return false;
	}

	int32_t files_size = 0;
	serializer.read_int("files_size", files_size);
	for (int32_t i = 0; i < files_size; i++) {
		String file;
		serializer.read_string("file", file);
		file = output_dir + "/" + file;

		int32_t file_data_size = 0;
		serializer.read_int("file_data_size", file_data_size);

		String file_data;
		file_data.resize(file_data_size);
		serializer.read_byte_array("file_data", file_data.data(), file_data_size);

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
