#pragma once

#include "utility/gui_util.h"
#include "utility/string_util.h"


namespace titian {
    enum class FileType
    {
        DEFAULT = 0,
        MESH,
        TEXTURE,
        SCRIPT,
        SHADER,
        SCENE,
    };

    inline FileType classify_file(const fs::path& file)
    {
        const String extension = file.extension().string();
        if (extension == FILE_EXTENSION_OBJ || extension == FILE_EXTENSION_GLB || extension == FILE_EXTENSION_FBX) {
            return FileType::MESH;
        }
        else if (extension == FILE_EXTENSION_JPG || extension == FILE_EXTENSION_PNG || extension == FILE_EXTENSION_BMP) {
            return FileType::TEXTURE;
        }
        else if (extension == FILE_EXTENSION_CHAI || extension == FILE_EXTENSION_DLL) {
            return FileType::SCRIPT;
        }
        else if (extension == FILE_EXTENSION_HLSL) {
            return FileType::SHADER;
        }
        else if (extension == FILE_EXTENSION_TITIAN) {
            return FileType::SCENE;
        }
        return FileType::DEFAULT;
    }

    inline Map<String, String> parse_ini_file(const String& path)
    {
        Map<String, String> data{};
        for (auto& line : kl::split_string(kl::read_file_string(path), '\n')) {
            const Vector<String> parts = kl::split_string(line, '=');
            if (parts.size() == 2) {
                const String key = trim(parts[0]);
				const String value = trim(parts[1]);
				data[key] = value;
            }
        }
        return data;
    }
}
