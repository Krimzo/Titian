#include "memory/files/file.h"

#include "utility/utility.h"


// Helper
std::string kl::files::get_extension(const std::string& filepath)
{
    return std::filesystem::path(filepath).extension().string();
}

std::vector<std::string> kl::files::get_files(const std::string& path, const bool recursive)
{
    std::vector<std::string> files = {};
    if (!recursive) {
        for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (!file.is_directory()) {
                files.push_back(file.path().string());
            }
        }
    }
    else {
        for (const auto& file : std::filesystem::recursive_directory_iterator(path)) {
            if (!file.is_directory()) {
                files.push_back(file.path().string());
            }
        }
    }
    return files;
}

std::string kl::files::read_string(const std::string& filepath)
{
    std::ifstream stream(filepath);
    if (warning_check(!stream.is_open(), "Failed to open file \"" + filepath + "\"")) {
        return {};
    }

    std::stringstream buffer = {};
    buffer << stream.rdbuf();
    stream.close();
    return buffer.str();
}

bool kl::files::write_string(const std::string& filepath, const std::string& data)
{
    std::ofstream stream(filepath);
    if (warning_check(!stream.is_open(), "Failed to open file \"" + filepath + "\"")) {
        return false;
    }

    stream << data;
    stream.close();
    return true;
}

bool kl::files::append_string(const std::string& filepath, const std::string& data, const int position)
{
    std::fstream stream(filepath, std::ios::in | std::ios::out);
    if (warning_check(!stream.is_open(), "Failed to open file \"" + filepath + "\"")) {
        return false;
    }

    if (position < 0) {
        stream.seekp(0, std::ios_base::end);
    }
    else {
        stream.seekp(position);
    }

    stream << data;
    stream.close();
    return true;
}

std::vector<kl::vertex> kl::files::parse_mesh(const std::string& filepath, const bool flip_z)
{
    std::fstream stream = {};
    stream.open(filepath, std::ios::in);
    if (warning_check(!stream.is_open(), "Failed to open file \"" + filepath + "\"")) {
        return {};
    }

    std::vector<vertex> vertex_data = {};
    std::vector<float3>    xyz_data = {};
    std::vector<float2>     uv_data = {};
    std::vector<float3> normal_data = {};

    const float z_flip = (flip_z ? -1.0f : 1.0f);

    for (std::string line; std::getline(stream, line);) {
        if (const std::vector<std::string> parts = strings::split(line, ' '); parts[0] == "v") {
            xyz_data.emplace_back(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]) * z_flip);
        }
        else if (parts[0] == "vt") {
            uv_data.emplace_back(std::stof(parts[1]), std::stof(parts[2]));
        }
        else if (parts[0] == "vn") {
            normal_data.emplace_back(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]) * z_flip);
        }
        else if (parts[0] == "f") {
            for (int i = 1; i < 4; i++) {
                const std::vector<std::string> line_part_parts = strings::split(parts[i], '/');
                vertex_data.emplace_back(
                    xyz_data[std::stoull(line_part_parts[0]) - 1],
                    uv_data[std::stoull(line_part_parts[1]) - 1],
                    normal_data[std::stoull(line_part_parts[2]) - 1]
                );
            }
        }
    }

    stream.close();
    return vertex_data;
}

// File
kl::file::file()
{}

kl::file::file(const std::string& filepath, const bool clear)
{
    open(filepath, clear);
}

kl::file::~file()
{
    close();
}

kl::file::operator bool() const
{
    return (bool) file_;
}

bool kl::file::open(const std::string& filepath, bool clear)
{
    close();
    const bool result = (bool) fopen_s(&file_, filepath.c_str(), clear ? "wb+" : "ab+");
    return !warning_check(result, "Failed to open file \"" + filepath + "\"");
}

void kl::file::close()
{
    if (file_) {
        fclose(file_);
        file_ = nullptr;
    }
}

bool kl::file::seek(const int position) const
{
    if (!file_) {
        return false;
    }
    if (position < 0) {
        return !fseek(file_, position + 1, SEEK_END);
    }
    return !fseek(file_, position, SEEK_SET);
}

bool kl::file::move(const int delta) const
{
    if (!file_) { return false; }
    return !fseek(file_, delta, SEEK_CUR);
}

bool kl::file::rewind() const
{
    return seek(0);
}

bool kl::file::unwind() const
{
    return seek(-1);
}

int kl::file::tell() const
{
    if (!file_) { return -1; }
    return (int) ftell(file_);
}
