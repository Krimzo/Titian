#include "klib.h"


// Helper
std::string kl::file_extension(const std::string& filepath)
{
    return std::filesystem::path(filepath).extension().string();
}

std::vector<std::string> kl::list_files(const std::string& path, const bool recursive)
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

std::string kl::read_file_string(const std::string& filepath)
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

bool kl::write_file_string(const std::string& filepath, const std::string& data)
{
    std::ofstream stream(filepath);
    if (warning_check(!stream.is_open(), "Failed to open file \"" + filepath + "\"")) {
        return false;
    }

    stream << data;
    stream.close();
    return true;
}

bool kl::append_file_string(const std::string& filepath, const std::string& data, const int position)
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

std::vector<kl::vertex> kl::parse_obj_file(const std::string& filepath, const bool flip_z)
{
    std::fstream file = {};
    file.open(filepath, std::ios::in);
    if (warning_check(!file.is_open(), "Failed to open file \"" + filepath + "\"")) {
        return {};
    }

    const float z_flip = (flip_z ? -1.0f : 1.0f);

    std::vector<float3>   world_data = {};
    std::vector<float2> texture_data = {};
    std::vector<float3>  normal_data = {};

    std::vector<vertex> vertex_data = {};

    for (std::string line; std::getline(file, line);) {
        const std::vector<std::string> parts = split_string(line, ' ');

        if (parts.size() == 4 && parts.front() == "v") {
            float3 result = {};
            result.x = strtof(parts[1].c_str(), nullptr);
            result.y = strtof(parts[2].c_str(), nullptr);
            result.z = strtof(parts[3].c_str(), nullptr) * z_flip;
            world_data.push_back(result);
        }
        
        if (parts.size() == 3 && parts.front() == "vt") {
            float2 result = {};
            result.x = strtof(parts[1].c_str(), nullptr);
            result.y = strtof(parts[2].c_str(), nullptr);
            texture_data.push_back(result);
        }
        
        if (parts.size() == 4 && parts.front() == "vn") {
            float3 result = {};
            result.x = strtof(parts[1].c_str(), nullptr);
            result.y = strtof(parts[2].c_str(), nullptr);
            result.z = strtof(parts[3].c_str(), nullptr) * z_flip;
            normal_data.push_back(result);
        }
        
        if (parts.size() == 4 && parts.front() == "f") {
            for (int i = 1; i < 4; i++) {
                const std::vector<std::string> line_part_parts = split_string(parts[i], '/');
                if (line_part_parts.size() != 3) { continue; }

                vertex vertex = {};
                if (uint64_t index = (strtoull(line_part_parts[0].c_str(), nullptr, 10) - 1); index >= 0 && index < world_data.size()) {
                    vertex.world = world_data[index];
                }
                if (uint64_t index = (strtoull(line_part_parts[1].c_str(), nullptr, 10) - 1); index >= 0 && index < texture_data.size()) {
                    vertex.texture = texture_data[index];
                }
                if (uint64_t index = (strtoull(line_part_parts[2].c_str(), nullptr, 10) - 1); index >= 0 && index < normal_data.size()) {
                    vertex.normal = normal_data[index];
                }
                vertex_data.push_back(vertex);
            }
        }
    }

    file.close();
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
