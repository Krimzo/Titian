#pragma once

#include "utility/gui_util.h"


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

    inline FileType classify_file(const std::filesystem::path& file)
    {
        const std::string extension = file.extension().string();
        if (extension == FILE_EXTENSION_MESH) {
            return FileType::MESH;
        }
        else if (extension == FILE_EXTENSION_JPG || extension == FILE_EXTENSION_PNG) {
            return FileType::TEXTURE;
        }
        else if (extension == FILE_EXTENSION_NATIVE_SCRIPT || extension == FILE_EXTENSION_INTER_SCRIPT) {
            return FileType::SCRIPT;
        }
        else if (extension == FILE_EXTENSION_SHADER) {
            return FileType::SHADER;
        }
        else if (extension == FILE_EXTENSION_SCENE) {
            return FileType::SCENE;
        }
        return FileType::DEFAULT;
    }
}
