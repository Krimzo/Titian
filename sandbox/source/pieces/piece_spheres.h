#pragma once

#include "sandbox_piece.h"


namespace titian {
    struct SandboxPieceSpheres : SandboxPiece
    {
        int size = 0;
        float x_offset = 0.0f;

        SandboxPieceSpheres(TitianEditor& editor, int size);

        void setup_self() override;
    };
}
