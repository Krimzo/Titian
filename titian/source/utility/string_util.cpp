#include "titian.h"


titian::String titian::get_default_script()
{
	return kl::read_file("package/defaults/default_script.lua");
}

titian::String titian::get_default_material_shader()
{
	return kl::read_file("package/defaults/default_material_shader.hlsl");
}

titian::String titian::get_default_camera_shader()
{
	return kl::read_file("package/defaults/default_camera_shader.hlsl");
}

titian::String titian::format_byte_size(uint64_t byte_size)
{
	if (byte_size < 1024) {
		return std::to_string(byte_size) + " B";
	}

	byte_size /= 1024;
	if (byte_size < 1024) {
		return std::to_string(byte_size) + " KB";
	}

	byte_size /= 1024;
	if (byte_size < 1024) {
		return std::to_string(byte_size) + " MB";
	}

	byte_size /= 1024;
	return std::to_string(byte_size) + " GB";
}

titian::String titian::format_path(const fs::path& path)
{
	return "[" + fs::absolute(path).string() + "]";
}

titian::String titian::ltrim(const StringRef& source, str t)
{
	String data{ source };
	data.erase(0, data.find_first_not_of(t));
	return data;
}

titian::String titian::rtrim(const StringRef& source, str t)
{
	String data{ source };
	data.erase(data.find_last_not_of(t) + 1);
	return data;
}

titian::String titian::trim(const StringRef& source, str t)
{
	return ltrim(rtrim(source, t), t);
}

bool titian::str_find(const StringRef& haystack, const StringRef& needle)
{
	static constexpr auto test = [](char l, char r) { return std::tolower(l) == std::tolower(r); };
	return std::search(haystack.begin(), haystack.end(), needle.begin(), needle.end(), test) != haystack.end();
}
