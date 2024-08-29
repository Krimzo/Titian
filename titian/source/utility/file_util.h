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
        BINARY_SCENE,
        TEXT_SCENE,
    };

    inline FileType classify_file(const fs::path& file)
    {
        const String extension = file.extension().string();
        if (extension == FILE_EXTENSION_OBJ || extension == FILE_EXTENSION_GLB || extension == FILE_EXTENSION_FBX) {
            return FileType::MESH;
        }
        if (extension == FILE_EXTENSION_JPG || extension == FILE_EXTENSION_PNG || extension == FILE_EXTENSION_BMP) {
            return FileType::TEXTURE;
        }
        if (extension == FILE_EXTENSION_LUA || extension == FILE_EXTENSION_DLL) {
            return FileType::SCRIPT;
        }
        if (extension == FILE_EXTENSION_HLSL) {
            return FileType::SHADER;
        }
        if (extension == FILE_EXTENSION_TITIAN) {
            return FileType::BINARY_SCENE;
        }
        if (extension == FILE_EXTENSION_JSON) {
            return FileType::TEXT_SCENE;
        }
        return FileType::DEFAULT;
    }
}
