#include "klibrary.h"


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

std::vector<byte> kl::read_file(const std::string_view& filepath)
{
    kl::File file = { filepath, false };
    if (!file) {
        return {};
    }

    file.unwind();
    const int file_size = file.tell();
    file.rewind();

    std::vector<byte> result(file_size);
    file.read(result.data(), file_size);
    return result;
}

bool kl::write_file(const std::string_view& filepath, const std::vector<byte>& data)
{
    kl::File file = { filepath, true };
    if (!file) {
        return false;
    }
    file.write<byte>(data.data(), data.size());
    return true;
}

std::string kl::read_file_string(const std::string& filepath)
{
    std::ifstream stream(filepath);
    if (!verify(stream.is_open(), "Failed to open file \"" + filepath + "\"")) {
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
    if (!verify(stream.is_open(), "Failed to open file \"" + filepath + "\"")) {
        return false;
    }

    stream << data;
    stream.close();
    return true;
}

bool kl::append_file_string(const std::string& filepath, const std::string& data, const int position)
{
    std::fstream stream(filepath, std::ios::in | std::ios::out);
    if (!verify(stream.is_open(), "Failed to open file \"" + filepath + "\"")) {
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

std::vector<kl::Vertex<float>> kl::parse_obj_file(const std::string& filepath, const bool flip_z)
{
    // Handle file
    std::ifstream file{ filepath };
    if (!verify(file.is_open(), "Failed to open file \"" + filepath + "\"")) {
        return {};
    }

    // Buffers
    std::vector<Float3> world_data{};
    std::vector<Float2> texture_data{};
    std::vector<Float3> normal_data{};
    std::vector<Vertex<float>> vertex_data{};

    // Parse
    const float z_flip = flip_z ? -1.0f : 1.0f;
    for (std::string line; std::getline(file, line);) {
        // Split line
        const std::vector<std::string> parts = split_string(line, ' ');

        // Parse world
        if (parts.size() == 4 && parts.front() == "v") {
            Float3 result{};
            result.x = strtof(parts[1].c_str(), nullptr);
            result.y = strtof(parts[2].c_str(), nullptr);
            result.z = strtof(parts[3].c_str(), nullptr) * z_flip;
            world_data.push_back(result);
            continue;
        }

        // Parse texture
        if (parts.size() == 3 && parts.front() == "vt") {
            Float2 result{};
            result.x = strtof(parts[1].c_str(), nullptr);
            result.y = strtof(parts[2].c_str(), nullptr);
            texture_data.push_back(result);
            continue;
        }

        // Parse normal
        if (parts.size() == 4 && parts.front() == "vn") {
            Float3 result{};
            result.x = strtof(parts[1].c_str(), nullptr);
            result.y = strtof(parts[2].c_str(), nullptr);
            result.z = strtof(parts[3].c_str(), nullptr) * z_flip;
            normal_data.push_back(result);
            continue;
        }

        // Parse face
        if (parts.size() == 4 && parts.front() == "f") {
            for (int i = 1; i <= 3; i++) {
                // Split part
                const std::vector<std::string> line_part_parts = split_string(parts[i], '/');
                if (line_part_parts.size() != 3) {
                    continue;
                }

                Vertex vertex{};
                if (uint64_t index = strtoull(line_part_parts[0].c_str(), nullptr, 10) - 1; index >= 0 && index < world_data.size()) {
                    vertex.world = world_data[index];
                }
                if (uint64_t index = strtoull(line_part_parts[1].c_str(), nullptr, 10) - 1; index >= 0 && index < texture_data.size()) {
                    vertex.texture = texture_data[index];
                }
                if (uint64_t index = strtoull(line_part_parts[2].c_str(), nullptr, 10) - 1; index >= 0 && index < normal_data.size()) {
                    vertex.normal = normal_data[index];
                }
                vertex_data.push_back(vertex);
            }
        }
    }
    return vertex_data;
}

std::optional<std::string> kl::choose_file(const bool save, const std::vector<std::string>& filters)
{
    std::stringstream filter_buffer = {};
    for (const auto& filter : filters) {
        filter_buffer << filter << '\0';
    }
    const std::string filter_data = filter_buffer.str();

    char file_buffer[250] = {};
    OPENFILENAMEA dialog_info = {};
    dialog_info.lStructSize = sizeof(dialog_info);
    dialog_info.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    dialog_info.lpstrFile = file_buffer;
    dialog_info.nMaxFile = sizeof(file_buffer);
    dialog_info.lpstrFilter = filter_data.c_str();
    dialog_info.nFilterIndex = 1;

    if (!save && GetOpenFileNameA(&dialog_info)) {
        return { { file_buffer } };
    }
    if (save && GetSaveFileNameA(&dialog_info)) {
        return { { file_buffer } };
    }
    return {};
}

// File
kl::File::File()
{}

kl::File::File(const std::string_view& filepath, const bool clear)
{
    open(filepath, clear);
}

kl::File::~File()
{
    close();
}

kl::File::operator bool() const
{
    return (bool) m_file;
}

void kl::File::open(const std::string_view& filepath, bool clear)
{
    close();
    errno_t error = fopen_s(&m_file, filepath.data(), clear ? "wb+" : "ab+");
    verify(error == NULL, kl::format("Failed to open file \"", filepath, "\""));
}

void kl::File::close()
{
    if (m_file) {
        fclose(m_file);
        m_file = nullptr;
    }
}

bool kl::File::seek(const int position) const
{
    if (!m_file) {
        return false;
    }
    if (position < 0) {
        return !fseek(m_file, position + 1, SEEK_END);
    }
    return !fseek(m_file, position, SEEK_SET);
}

bool kl::File::move(const int delta) const
{
    if (!m_file) { return false; }
    return !fseek(m_file, delta, SEEK_CUR);
}

bool kl::File::rewind() const
{
    return seek(0);
}

bool kl::File::unwind() const
{
    return seek(-1);
}

int kl::File::tell() const
{
    if (!m_file) { return -1; }
    return (int) ftell(m_file);
}
