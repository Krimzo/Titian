#pragma once

#include "sandbox_piece.h"


namespace titian {
    struct SandboxPieceTest : SandboxPiece
    {
        SandboxPieceTest(TitianEditor* editor);

        void setup_self() override;
    };
}
