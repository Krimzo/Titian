#pragma once

#include "utility/gui_util.h"
#include "utility/string_util.h"


namespace titian {
    enum struct FileType : int32_t
    {
        DEFAULT = 0,
        MESH,
        TEXTURE,
        SCRIPT,
        SHADER,
        BINARY_SCENE,
        TEXT_SCENE,
    };
}

namespace titian {
    FileType classify_file(const fs::path& file);
}
